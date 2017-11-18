#include <iostream>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>


const GLint WIDTH = 800, HEIGHT = 600;

// 셰이더 를 당장은 직접 코드내에 삽입

const GLchar *vertexShaderSource = "#version 330 core\n"
"layout ( location = 0 ) in vec3 position;\n"
"void main()\n"
"{\n"
"gl_Position = vec4( position.x, position.y, position.z, 1.0);\n"
"}";

const GLchar *fragmentShaderSource = "#version 330 core\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"color = vec4(0.1f, 0.3f, 0.8f, 1.0f);\n"
"}";

int main()
{
    // Init GLFW
    glfwInit();
    
    // GLFW 관련 필수 설정
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
    
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE,GL_FALSE);
    
    // GLFW 함수를 사용하기 위해 GLFW 윈도우 오브젝트를 생성
    GLFWwindow *window = glfwCreateWindow(WIDTH,HEIGHT, "Learn OpenGL",nullptr,nullptr);
    
    int screenWidth, screenHeight;
    
    glfwGetFramebufferSize(window,&screenWidth,&screenHeight);
    
    if(nullptr == window)
    {
        std::cout << "Failed to create GLFW Window" << std::endl;
        
        glfwTerminate();
        return -1;
    }
    
    glfwMakeContextCurrent(window);
    
    glewExperimental = GL_TRUE;
    
    if(GLEW_OK != glewInit())
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        
        return -1;
    }
    
    glViewport(0,0,screenWidth,screenHeight);
    
    // 버텍스 셰이더 컴파일
    
    // 버텍스 셰이더 생성
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader,1,&vertexShaderSource,NULL);
    // 컴파일
    glCompileShader(vertexShader);
    
    // 컴파일 성공 여부
    GLint success;
    GLchar infoLog[512];
    
    // 셰이더 오브젝트로 부터 원하는 파라미터를 가져온다
    glGetShaderiv(vertexShader,GL_COMPILE_STATUS,&success);
    
    if(!success)
    {
        // glGetShaderInfoLog - 셰이더 오브젝트의 인포 로그를 리턴
        // GLuint shader - 질문할 대상 셰이더 오브젝트
        // GLsizei maxLength - 정보 로그를 저장할 문자 버퍼의 사이즈
        // GLsizei *length - infoLog 에 담겨나간 문자가 몇개 나왔는지 알려줄 길이 값을 담을 컨테이너
        // GLchar *infoLog - 로그를 담아갈 문자열 컨테이너
        glGetShaderInfoLog(vertexShader,512,NULL,infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    
    // 프래그먼트 셰이더 컴파일
    
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader,1,&fragmentShaderSource,NULL);
    glCompileShader(fragmentShader);
    
    glGetShaderiv(fragmentShader,GL_COMPILE_STATUS, &success);
    
    if(!success)
    {
        glGetShaderInfoLog(fragmentShader,512,NULL,infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    
    // 셰이더 프로그램 생성
    GLuint shaderProgram = glCreateProgram();
    
    // 셰이더 첨부와 링크 완료
    glAttachShader(shaderProgram,vertexShader);
    glAttachShader(shaderProgram,fragmentShader);
    
    glLinkProgram(shaderProgram);
    
    // 로그 찍자
    glGetProgramiv(shaderProgram,GL_LINK_STATUS,&success);
    
    if(!success)
    {
        glGetProgramInfoLog(shaderProgram,512,NULL,infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    
    // 셰이더 프로그램에 적용했으니 사용한 셰이더는 이제 버려도 됨
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    // 삼각형을 위한 버텍스 세개
    GLfloat vertices[] =
    {
        -0.5f,-0.5f,0.0f, // 바닥 왼쪽
        0.5f,-0.5f,0.0f, // 바닥 오른쪽
        0.0f, 0.5f, 0.0f // 위쪽
        
    };
    
    // VBO와 VAO 의 차이는 다음번에 보기로 하자
    GLuint VBO,VAO;
    
    glGenVertexArrays(1,&VAO);
    glGenBuffers(1,&VBO);
    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE, 3* sizeof(GLfloat),(GLvoid *) 0);
    
    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER,0);
    
    glBindVertexArray(0);
    
    // 드로잉 루프
    while(!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        glClearColor(0.2f,0.3f,0.3f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        // 어떤 셰이더 프로그램을 사용할 것인가 가르킴
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES,0,3);
        glBindVertexArray(0);
        
        glfwSwapBuffers(window);
    }
    
    // 윈도우 종료시 모든것을 할당 해제
    // 버텍스를 할당했던 인덱스를 지정해줌
    glDeleteVertexArrays(1,&VAO);
    glDeleteBuffers(1,&VBO);
    
    glfwTerminate();
    
    return 0;
    
}
