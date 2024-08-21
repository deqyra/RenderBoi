#ifndef UNIFORM_BLOCKS_LIGHTS
#define UNIFORM_BLOCKS_LIGHTS

#include </interface_blocks/light_types>

#define       POINT_MAX_COUNT 64
#define        SPOT_MAX_COUNT 64
#define DIRECTIONAL_MAX_COUNT  4

// THIS UNIFORM BLOCK (including the macros above) MUST BE KEPT IN SYNC WITH renderboi/core/ubo/light_ubo.hpp

layout (std140, binding = 1) uniform Lights {               // Size    // Align // Offset
	PointLight point[POINT_MAX_COUNT];                      // 64 * 64 //    16 //      0
	uint pointCount;                                        //       4 //     4 //   4096

	SpotLight spot[SPOT_MAX_COUNT];                         // 64 * 80 //    16 //   4112
	uint spotCount;                                         //       4 //     4 //   9232
	
	DirectionalLight direct[DIRECTIONAL_MAX_COUNT];         //  4 * 64 //    16 //   9248
	uint directionalCount;                                  //       4 //     4 //   9504
} lights;                                                   // Size: 9508

#endif//UNIFORM_BLOCKS_LIGHTS