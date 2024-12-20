# ctle Copyright (c) 2023 Ulrik Lindahl
# Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE

import copy
import os
from stat import S_IRUSR, S_IRGRP, S_IROTH, S_IWUSR
import importlib
import textwrap
from contextlib import contextmanager

class formatted_output:
	'''formatted_output defined a class which can be used to more easily generate formatted output of C/C++ files. The class supports different kinds of indentation, and wraps comments.'''

	def __init__(self, indentation:int = 0, tab_str:str = '\t', indent_braces:bool = False, comment_wrap:int = 120 ) -> None:
		self.lines = []
		self.indent_braces = indent_braces
		self.indentation = indentation
		self.tab_str = tab_str
		self.comment_wrap = comment_wrap

	@contextmanager
	def blk(self, add_semicolon:bool = False, no_indent = False):
		'''Add an indented block of code'''
		try:
			st = self.tab_str * self.indentation
			if self.indent_braces:
				st += self.tab_str
			st += '{'
			self.lines.append(st)
			if not no_indent:
				self.indentation += 1	
			yield self

		finally:
			if not no_indent:
				self.indentation -= 1
			st = self.tab_str * self.indentation
			if self.indent_braces:
				st += self.tab_str 
			st += '}'
			if add_semicolon:
				st += ';'
			self.lines.append(st)

	@contextmanager
	def tab(self, prefix_ln:str = None):
		'''Add an indented block of code, only indent, no braces'''
		try:
			if prefix_ln:
				self.ln(prefix_ln)
			self.indentation += 1	
			yield self

		finally:
			self.indentation -= 1

	def ln(self, lin:str = None, no_indent = False, append = False, intent_adjust = 0 ):
		'''Add a single line, at current indentation, (or no indentation if no_indent is True).'''
		if append:
			if lin:
				self.lines[-1] += lin
		else:				
			if lin != None:
				if no_indent:
					self.lines.append( lin )
				else:
					self.lines.append( self.tab_str * (self.indentation + intent_adjust) + lin )
			else:
				self.lines.append('')
   
	# single line block
	def blk_ln(self, lin:str, add_comma:bool = False ):
		'''Add a single-line block, at the current indentation. Optionally add a comma at the end.'''
		st = ''
		if self.indent_braces:
			st += self.tab_str
		st += '{ ' + lin + ' }'
		if add_comma:
			st += ','
		self.ln( st )

	def comment_ln(self, comm:str):
		'''Add a commend, which is wrapped if longer than the comment_wrap length'''
		lines = textwrap.wrap( comm , width = self.comment_wrap )
		for lin in lines:
			self.ln('// ' + textwrap.dedent(lin) )

	def generate_license_header(self, project_name:str='ctle', copyright_holder:str = '2022 Ulrik Lindahl', license_type:str = 'MIT', license_link:str = 'https://github.com/Cooolrik/ctle/blob/main/LICENSE' ):
		'''Generates the license header and injects into the generated c++ file'''
		self.comment_ln(f'{project_name} Copyright (c) {copyright_holder}')
		self.comment_ln(f'Licensed under the {license_type} license {license_link}')

	def begin_header_guard(self, file_name:str, project_name:str='ctle') -> str:
		'''Beings the header guard for the file'''
		def todef(s:str):
			return s.upper().replace("-", "_").replace(" ", "_").replace(".", "_")
		guard_def = todef(f'_{project_name}_{file_name}_')
		self.ln('#pragma once')
		self.ln(f'#ifndef {guard_def}')
		self.ln(f'#define {guard_def}')
		return guard_def

	def end_header_guard(self, guard_def:str):
		'''Ends the header guard for the file'''
		self.ln(f'#endif//{guard_def}')

	def inline_file(self, path:str):
		'''Injects the contents of another file, indented at the current indentation level'''
		inlined_file = open(path, 'r')
		lines = inlined_file.readlines()
		for str in lines:
			self.ln( str.rstrip() )
		inlined_file.close()

	def generate_push_and_disable_warnings(self, vswarns:list[str] , gccwarns:list[str] , comment:str = 'Disable warnings that are caused by code which we cannot control' ):
		'''Insert a block of warning disable pragmas for VC and GCC. Pushes all warnings before disabling.'''
		self.comment_ln(comment)
		self.ln('#ifdef _MSC_VER')
		self.ln('#pragma warning( push )')
		for w in vswarns:
			self.ln(f'#pragma warning( disable : {w} )')
		self.ln('#elif defined(__GNUC__)')
		self.ln('#pragma GCC diagnostic push')
		for w in gccwarns:
			self.ln(f'#pragma GCC diagnostic ignored "{w}"')	
		self.ln('#endif')

	def generate_pop_warnings(self, comment:str = 'Re-enable warnings again' ):
		'''Insert a block which pops the disabled warnings for VC or GCC'''
		self.comment_ln(comment)
		self.ln('#ifdef _MSC_VER')
		self.ln('#pragma warning( pop )')
		self.ln('#elif defined(__GNUC__)')
		self.ln('#pragma GCC diagnostic pop')
		self.ln('#endif')

	def write_lines_to_file( self, path:str ):
		'''Write the formatted text to a file, but only if the file is not already exactly identical to the formatted text'''

		# make into one long string
		new_text = ''
		for line in self.lines:
			new_text += line + '\n'

		# if the file already exists, check if we have an update
		if os.path.exists(path):
			# try reading in the current file if one exists
			with open(path,'r') as f:
				existing_text = f.read()
				f.close()

			#if no difference was found, return
			if new_text == existing_text:
				print( '\tSkipping: ' + path + ', (it is identical)...')
				return

			# if a difference was found, remove the old file
			os.chmod(path, S_IWUSR)
			os.remove( path ) 

		# we should write to the new file
		print( '\tWriting: ' + path + '...')
		with open(path,'w') as f:
			f.write(new_text)
			f.close()

		# change mode of file to readonly
		os.chmod(path, S_IRUSR|S_IRGRP|S_IROTH)


