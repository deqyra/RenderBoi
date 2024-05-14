#ifndef INTERFACE_BLOCKS_LIGHT_TYPES
#define INTERFACE_BLOCKS_LIGHT_TYPES

// Offset calculation:
//    base offset = offset of previous member + size of previous member
// aligned offset = base offset rounded up to the nearest multiple of the alignment constraint for this member
// aligned offset = prev. offset + prev. size + padding to align
//
// members are interleaved to minimize padding
// padding is done manually where it is needed, so implicit padding bytes are always 0 and is omitted from calculation details

// THIS STRUCT MUST BE KEPT IN SYNC WITH renderboi/core/ubo/layout/point_light.hpp
// THIS STRUCT IS USED IN THE LIGHT UBO DECLARED IN assets/shaders/uniform_blocks/lights.glsl

struct PointLight {                         // Size // Align // Offset
    vec3 position;                          // 12   // 16    //      0
    float _padding1;                        //  4   //  4    //     12

    vec3 ambient;                           // 12   // 16    //     16
    float constant;                         //  4   //  4    //     28

    vec3 diffuse;                           // 12   // 16    //     32
    float linear;                           //  4   //  4    //     44

    vec3 specular;                          // 12   // 16    //     48
    float quadratic;                        //  4   //  4    //     60
};                                          // Total size: 64; alignment: 16

// THIS STRUCT MUST BE KEPT IN SYNC WITH renderboi/core/ubo/layout/spot_light.hpp
// THIS STRUCT IS USED IN THE LIGHT UBO DECLARED IN assets/shaders/uniform_blocks/lights.glsl

struct SpotLight {                          // Size // Align // Offset
    vec3 position;                          // 12   // 16    //      0
	float innerCutoff;                      //  4   //  4    //     12

    vec3 direction;                         // 12   // 16    //     16
	float outerCutoff;                      //  4   //  4    //     28

    vec3 ambient;                           // 12   // 16    //     32
    float constant;                         //  4   //  4    //     44

    vec3 diffuse;                           // 12   // 16    //     48
    float linear;                           //  4   //  4    //     60

    vec3 specular;                          // 12   // 16    //     64
    float quadratic;                        //  4   //  4    //     76
};                                          // Total size: 80; alignment: 16

// THIS STRUCT MUST BE KEPT IN SYNC WITH renderboi/core/ubo/layout/directional_light.hpp
// THIS STRUCT IS USED IN THE LIGHT UBO DECLARED IN assets/shaders/uniform_blocks/lights.glsl

struct DirectionalLight {                   // Size // Align // Offset
    vec3 direction;                         // 12   // 16    //      0
    float _padding1;                        //  4   //  4    //     12

    vec3 ambient;                           // 12   // 16    //     16
    float _padding2;                        //  4   //  4    //     28

    vec3 diffuse;                           // 12   // 16    //     32
    float _padding3;                        //  4   //  4    //     44

    vec3 specular;                          // 12   // 16    //     48
    float _padding4;                        //  4   //  4    //     60
};                                          // Total size: 64; alignment: 16

#endif//INTERFACE_BLOCKS_LIGHT_TYPES