//***************************************************************************
//
//  File Name :		MSGEQ7.h
//
//  Project :		Arduino style libraries
//
//  Purpose :		Wrapper for the MSGEQ7 7 band graphic equaliser
//
// The MIT License (MIT)
//
// Copyright (c) 2017-2023 Andy Burgess
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
//  Revisions :
//
//      see rcs below
//
//***************************************************************************

#ifndef __MSGEQ7_H__
#define __MSGEQ7_H__


/// @brief MSGEQ7 interface class
/// @details A template class to interface with a MSGEQ7 graphic equaliser.
class Msgeq7
{
//fields
public:
	static const uint8_t maxBands = 7;
protected:
private:
	uint8_t _rst;
	uint8_t _stb;
	uint8_t _ana;
	uint16_t _data[maxBands];

//methods
public:
    /// @brief Initialises a new instance of the Msgeq7 class with the specified
	/// pins.
    /// @param rst The reset pin.
    /// @param stb The strobe pin.
    /// @param ana The analogue pin.
    Msgeq7 (uint8_t rst, uint8_t stb, uint8_t ana) : _rst(rst), _stb(stb), _ana(ana)
	{
		pinMode(_rst, OUTPUT);
		pinMode(_stb, OUTPUT);
		pinMode(_ana, INPUT);
		reset();
	}

	//~Msgeq7();

	/// @brief Resets the MSGEQ7 device.
	void reset()
	{
		// Datasheet shows 100ns min for the active high reset pulse then delay for 72us
		digitalWrite(_rst, HIGH);
		delayMicroseconds(1);
		digitalWrite(_rst, LOW);
	}


	/// @brief Reads the device, placing the values for each of the bands into a buffer.
	/// @param res True to reset before reading; otherwise false for no reset.
	void read(bool res = false)
	{
		if (res) reset();

		// The read cycle (from the datasheet) is take STB high for minimum of 18us, 
		// take strobe low and the data will be available 36us later.   Strobe to strobe
		// delay is 72us
		for (uint8_t i = 0; i < maxBands; i++)
		{
			digitalWrite(_stb, HIGH);
			delayMicroseconds(18);
			digitalWrite(_stb, LOW);
			delayMicroseconds(36);
			_data[i] = analogRead(_ana);
			delayMicroseconds(18);
		}
	}


	/// @brief Retrieve the value of the specified channel / band from the buffer.
	/// @param chan The channel to read (0-6).
	/// @return The value of the channel.
	uint16_t get(const uint8_t chan)
	{
		if (chan >= maxBands) return 0;

		return _data[chan];
	}

	protected:
	private:
		Msgeq7( const Msgeq7 &c );
		Msgeq7& operator=( const Msgeq7 &c );
}; //Msgeq7

#endif //__MSGEQ7_H__
