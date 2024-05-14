#ifndef UNIFORM_BLOCKS_MATRICES
#define UNIFORM_BLOCKS_MATRICES

// THIS UNIFORM BLOCK MUST BE KEPT IN SYNC WITH renderboi/core/ubo/matrix_ubo.hpp

layout (std140, binding = 0) uniform Matrices {       // Size // Align // Offset
    mat4 view;                                        //   64 //    64 //      0
    mat4 projection;                                  //   64 //    64 //     64
    mat4 model;                                       //   64 //    64 //    128
    mat3 normal;                                      //   48 //    64 //    192
} matrices;                                           // Size: 240

#endif//UNIFORM_BLOCKS_MATRICES