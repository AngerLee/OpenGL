#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

const float triangle[] = {
    -0.5f,-0.5f,0.0f,
    0.5f,-0.5f,0.0f,
    0.0f,0.5f,0.0f,
};

// 顶点着色器和片段着色器源码
const char *vertex_shader_source = 
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos,1.0);\n"
    "}\n\0";

const char *fragment_shader_source = 
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(0.0f,0.5f,0.0f,1.0f);\n"
    "}\n\0";

int main(){
    // 初始化 <-------Start
    // 初始化GLFW
    glfwInit();
    // OpenGL版本为3.3，主版本号为3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    // OpenGL版本为3.3，次版本号为3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    // 使用核心模式
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
    // 不可改变窗口大小
    glfwWindowHint(GLFW_RESIZABLE,false);
    // 窗口宽，高
    int screen_width = 1280;
    int screen_height = 720;
    // 创建窗口
    auto window = glfwCreateWindow(screen_width,screen_height,"Computer Graphics",nullptr,nullptr);
    // 如果创建失败，则报错
    if(window == nullptr){
        std::cout << "Failed to Create OpenGL Context" << std::endl;
        glfwTerminate();
        return -1;
    }

    // 将窗口的上下文设置为当前线程的主上下文
    glfwMakeContextCurrent(window);
    // 初始化GLAD,加载OpenGL函数指针地址的函数
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "Failed to initialize GLAD" << std::endl;
    }
    // 指定当前视窗尺寸
    glViewport(0,0,screen_width,screen_height);
    // 初始化 <-------end

    GLuint vertex_array_object; // VAO
    glGenVertexArrays(1,&vertex_array_object);
    glBindVertexArray(vertex_array_object);
    GLuint vertex_buffer_object; // VBO
    glGenBuffers(1,&vertex_buffer_object);
    glBindBuffer(GL_ARRAY_BUFFER,vertex_buffer_object);
    // 将顶点数据绑定到当前默认的缓冲中
    glBufferData(GL_ARRAY_BUFFER,sizeof(triangle),triangle,GL_STATIC_DRAW);
    // 设置顶点属性指针
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);
    // 解绑VAO和VBO
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER,0);

    // 生成并且编译着色器
    // 顶点着色器
    int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader,1,&vertex_shader_source,NULL);
    glCompileShader(vertex_shader);
    int success;
    char info_log[512];
    glGetShaderiv(vertex_shader,GL_COMPILE_STATUS,&success);
    // 检查着色器是否成功编译，如果编译失败，打印错误信息
    if(!success){
        glGetShaderInfoLog(vertex_shader,512,NULL,info_log);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED \n" << info_log << std::endl;
    }

    // 生成并且编译着色器
    // 片段着色器
    int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader,1,&fragment_shader_source,NULL);
    glCompileShader(fragment_shader);
    glGetShaderiv(fragment_shader,GL_COMPILE_STATUS,&success);
    // 检查着色器是否成功编译，如果编译失败，打印错误信息
    if(!success){
        glGetShaderInfoLog(vertex_shader,512,NULL,info_log);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED \n" << info_log << std::endl;
    }

    // 链接顶点和片段着色器到一个着色器程序
    int shader_program = glCreateProgram();
    glAttachShader(shader_program,vertex_shader);
    glAttachShader(shader_program,fragment_shader);
    glLinkProgram(shader_program);
    // 检查着色器是否成果链接，如果链接失败，打印错误信息
    glGetProgramiv(shader_program,GL_LINK_STATUS,&success);
    if(!success){
        glGetProgramInfoLog(shader_program,512,NULL,info_log);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED \n" << info_log << std::endl;
    }

    // 删除着色器
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    // 渲染阶段
    while(!glfwWindowShouldClose(window)){
        // 清空颜色缓冲
        glClearColor(0.0f,0.0f,0.0f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // 使用着色器程序
        glUseProgram(shader_program);

        // 绘制三角形
        glBindVertexArray(vertex_array_object); // 绑定VAO
        glDrawArrays(GL_TRIANGLES,0,3); // 绘制三角形
        glBindVertexArray(0); // 解除绑定

        // 交换缓冲区并且检查是否有触发事件
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // 删除VAO 和 VBO
    glDeleteVertexArrays(1,&vertex_array_object);
    glDeleteBuffers(1,&vertex_buffer_object);

    // 清理所有资源并且正确推出程序
    glfwTerminate();
    return 0;
}