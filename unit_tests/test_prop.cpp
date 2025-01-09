// ctle Copyright (c) 2023 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE

#include "../ctle/prop.h"
#include "../ctle/status_return.h"
#include "../ctle/status_error.h"

#include "unit_tests.h"

using namespace ctle;

class person
{
	int _weight = 0;
	int _height = 0;

public:
	person();

	prop_get_set<int> weight;
	prop_get_set<int> height;

	prop_set<float, prop::custom> height_in_meters;
	prop_get<float, prop::cpy_custom> bmi;

	const void *last_accessed_custom_prop = nullptr;
};

person::person()
	: weight( _weight )
	, height( _height )
	, height_in_meters( [this]( const prop_set<float, prop::custom> *prop, const float &value ) -> status
		{
			this->last_accessed_custom_prop = prop;
			this->_height = (int)( value * 100.f ); // convert to centimeters, write directly to height.v, skip the height prop method
			return status::ok;
		} )
	, bmi( [this]( const prop_get<float, prop::cpy_custom> *prop, status & ) -> float
		{
			this->last_accessed_custom_prop = prop;
			float height_m = (float)this->_height / 100.f;
			return (float)( this->_weight ) / ( height_m * height_m );
		} )
{}

TEST( prop, basic_test )
{
	person p;

	p.weight = 80;

	p.height_in_meters = 1.8f;
	EXPECT_EQ( p.last_accessed_custom_prop, &p.height_in_meters );

	EXPECT_EQ( p.height, 180 );

	float bmi = p.bmi;
	EXPECT_EQ( p.last_accessed_custom_prop, &p.bmi );
	EXPECT_NEAR( bmi, 24.6913f, 0.0001f );	// 80kg / (1.8m * 1.8m) = 24.6913 kg/m^2
}

class folks
{
private:
	std::vector<std::unique_ptr<person>> _persons;
	int _simple_int;

public:
	folks();

	prop_get<std::vector<std::unique_ptr<person>>, prop::custom> persons;

	prop_get<int> simple_int;

	void init_persons( int nums )
	{
		_persons.clear();
		_persons.resize( nums );
		for( size_t i = 0; i < (size_t)nums; ++i )
		{
			_persons[i] = std::unique_ptr<person>( new person() );
		}
		_simple_int = nums;
	}

};

folks::folks()
	: persons( [this]( const prop_get<std::vector<std::unique_ptr<person>>, prop::custom> *, status &result ) -> const std::vector<std::unique_ptr<person>> &
		{
			if( this->_persons.size() < 30 )
			{
				result = status::invalid; // set invalid as result, and return the local static value which has no value
				return nil_object::ref<std::vector<std::unique_ptr<person>>>();
			}
			return this->_persons;
		}
	)
	, simple_int( _simple_int, 23 )
{}

TEST( prop, complicated_property_test )
{
	folks f;

	// simple_int should default to 23
	EXPECT_EQ( (int)f.simple_int, 23 );

	// initialize to a low value
	f.init_persons( 20 );

	// trigger invalid status
	EXPECT_THROW( f.persons.get(), ctle::status_error );

	// use status parameter instead 
	status result;
	auto &tm = f.persons.get( result );
	EXPECT_EQ( result, status::invalid );

	// make sure that the value returned is not the actual value, but rather an empty static stand-in
	EXPECT_TRUE( nil_object::is_nil( tm ) );

	// initialize, so we are not invalid anymore
	f.init_persons( 60 );

	// after init, simple_int should be 145
	EXPECT_EQ( (int)f.simple_int, 60 );

	// make sure it is possible to access the values and that all values are set
	EXPECT_EQ( f.persons.get().size(), 60 );
	for( size_t inx = 0; inx < f.persons.get().size(); ++inx )
	{
		EXPECT_NE( f.persons.get()[20].get(), nullptr );
	}
}
