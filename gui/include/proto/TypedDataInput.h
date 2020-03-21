#pragma once

#include <stdint.h>
#include <float.h>

namespace spvgentwo
{
	class Type;
	class Constant;
}

namespace proto
{
	// for small scalars, vectors and matrices (upto 4x4)
	class TypedDataInput
	{
	public:
		TypedDataInput();
		~TypedDataInput();

		// returns true if type is valid / input was matched
		bool update(const spvgentwo::Type& _type);
		bool update(const spvgentwo::Type& _type, spvgentwo::Constant& _outConstant);

		auto get() const { return m_data; }
	private:

		const char* m_label = "Value";

		float m_speed = 1.f;
		float m_min = -(FLT_MAX);
		float m_max = FLT_MAX;
		const char* m_format = nullptr;
		float m_power = 1.f;

		bool m_drag = true;

		union 
		{
			//uint8_t u8;

			// 16 bit vec
			int16_t s16;
			int16_t s16v2[2];
			int16_t s16v3[3];
			int16_t s16v4[4];

			uint16_t u16;
			uint16_t u16v2[2];
			uint16_t u16v3[3];
			uint16_t u16v4[4];

			// 32 bit vec
			int32_t s32;
			int32_t s32v2[2];
			int32_t s32v3[3];
			int32_t s32v4[4];

			uint32_t u32;
			uint32_t u32v2[2];
			uint32_t u32v3[3];
			uint32_t u32v4[4];

			float f32;
			float f32v2[2];
			float f32v3[3];
			float f32v4[4];

			// 64 bit vec
			int64_t s64;
			int64_t s64v2[2];
			int64_t s64v3[3];
			int64_t s64v4[4];

			uint64_t u64;
			uint64_t u64v2[2];
			uint64_t u64v3[3];
			uint64_t u64v4[4];

			double f64;
			double f64v2[2];
			double f64v3[3];
			double f64v4[4];

			// 2x2
			int16_t s16m22[2][2];
			int32_t s32m22[2][2];
			int64_t s64m22[2][2];

			uint16_t u16m22[2][2];
			uint32_t u32m22[2][2];
			uint64_t u64m22[2][2];

			float f32m22[2][2];
			double f64m22[2][2];

			// 3x3
			int16_t s16m33[3][3];
			int32_t s32m33[3][3];
			int64_t s64m33[3][3];

			uint16_t u16m33[3][3];
			uint32_t u32m33[3][3];
			uint64_t u64m33[3][3];

			float f32m33[3][3];
			double f64m33[3][3];

			// todo: 3x4 and 4x3

			// 4x4
			int16_t s16m44[4][4];
			int32_t s32m44[4][4];
			int64_t s64m44[4][4];

			uint16_t u16m44[4][4];
			uint32_t u32m44[4][4];
			uint64_t u64m44[4][4];

			float f32m44[4][4];
			double f64m44[4][4];
		} m_data;
	};
}