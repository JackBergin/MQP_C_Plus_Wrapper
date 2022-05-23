/* License: Apache 2.0. See LICENSE file in root directory.
    Copyright(c) 2017 Intel Corporation. All Rights Reserved. */
    
    #ifndef LIBREALSENSE_RS2_PROCESSING_GL_H
    #define LIBREALSENSE_RS2_PROCESSING_GL_H
    
    #ifdef __cplusplus
    extern "C" {
    #endif
    
    #include "librealsense2/rs.h"
    
    /*
    * GL-specific extension types.
    * Used similar to regular RS extensions
    */
    typedef enum rs2_gl_extension
    {
        RS2_GL_EXTENSION_VIDEO_FRAME,
        RS2_GL_EXTENSION_COUNT
    } rs2_gl_extension;
    const char* rs2_gl_extension_to_string(rs2_extension type);
    
    /*
    * In addition to "options", GL processing blocks
    * expose new type of multidementional options each holding a 4x4 matrix
    */
    typedef enum rs2_gl_matrix_type
    {
        RS2_GL_MATRIX_TRANSFORMATION,
        RS2_GL_MATRIX_PROJECTION,
        RS2_GL_MATRIX_CAMERA,
        RS2_GL_MATRIX_COUNT
    } rs2_gl_matrix_type;
    const char* rs2_gl_matrix_type_to_string(rs2_gl_matrix_type type);
    
    /* Forward-declare GLFW types */
    typedef struct GLFWwindow GLFWwindow;
    typedef struct GLFWmonitor GLFWmonitor;
    typedef int(*glfwInitFun)(void);
    typedef void(*glfwWindowHintFun)(int, int);
    typedef GLFWwindow*(*glfwCreateWindowFun)(int, int, const char*, GLFWmonitor*, GLFWwindow*);
    typedef void(*glfwDestroyWindowFun)(GLFWwindow*);
    typedef void(*glfwMakeContextCurrentFun)(GLFWwindow*);
    typedef GLFWwindow*(*glfwGetCurrentContextFun)(void);
    typedef void(*glfwSwapIntervalFun)(int);
    typedef void(*GLFWglproc)(void);
    typedef GLFWglproc(*glfwGetProcAddressFun)(const char*);
    
    /* To avoid GLFW version mismatch (mainly can affect Windows), the application passes pointers
    * to its version of GLFW, via glfw_bindings struct
    */
    struct glfw_binding
    {
        glfwInitFun glfwInit;
        glfwWindowHintFun glfwWindowHint;
        glfwCreateWindowFun glfwCreateWindow;
        glfwDestroyWindowFun glfwDestroyWindow ;
        glfwMakeContextCurrentFun glfwMakeContextCurrent;
        glfwGetCurrentContextFun glfwGetCurrentContext;
        glfwSwapIntervalFun glfwSwapInterval;
        glfwGetProcAddressFun glfwGetProcAddress;
    };
    
    rs2_processing_block* rs2_gl_create_yuy_decoder(int api_version, rs2_error** error);
    
    rs2_processing_block* rs2_gl_create_y411_decoder(int api_version, rs2_error** error);
    
    void rs2_gl_set_matrix(rs2_processing_block* block, rs2_gl_matrix_type type, float* m4x4, rs2_error** error);
    
   int rs2_gl_is_frame_extendable_to(const rs2_frame* f, rs2_gl_extension extension_type, rs2_error** error);
   
   unsigned int rs2_gl_frame_get_texture_id(const rs2_frame* f, unsigned int id, rs2_error** error);
   
   rs2_processing_block* rs2_gl_create_camera_renderer(int api_version, rs2_error** error);
   
   rs2_processing_block* rs2_gl_create_pointcloud_renderer(int api_version, rs2_error** error);
   
   rs2_processing_block* rs2_gl_create_pointcloud(int api_version, rs2_error** error);
   
   rs2_processing_block* rs2_gl_create_uploader(int api_version, rs2_error** error);
   
   rs2_processing_block* rs2_gl_create_colorizer(int api_version, rs2_error** error);
   
   rs2_processing_block* rs2_gl_create_align(int api_version, rs2_stream to, rs2_error** error);
   
   void rs2_gl_init_rendering(int api_version, int use_glsl, rs2_error** error);
   
   void rs2_gl_init_rendering_glfw(int api_version, glfw_binding bindings, int use_glsl, rs2_error** error);
   
   void rs2_gl_init_processing(int api_version, int use_glsl, rs2_error** error);
   
   void rs2_gl_init_processing_glfw(int api_version, GLFWwindow* share_with, 
                                    glfw_binding bindings, int use_glsl, rs2_error** error);
   
   void rs2_gl_shutdown_rendering(int api_version, rs2_error** error);
   
   void rs2_gl_shutdown_processing(int api_version, rs2_error** error);
   
   #ifdef __cplusplus
   }
   #endif
   #endif
