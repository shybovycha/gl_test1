#include <GL/glew.h>
#include <GL/glu.h>
#include <GL/gl.h>

#include <stdio.h>

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#ifdef WIN32
    #ifdef _DEBUG
        #pragma comment(lib, "sfml-main-d.lib")
        #pragma comment(lib, "sfml-graphics-d.lib")
        #pragma comment(lib, "sfml-system-d.lib")
        #pragma comment(lib, "sfml-window-d.lib")
        #pragma comment(lib, "glew32.lib")
        #pragma comment(lib, "glu32.lib")
        #pragma comment(lib, "opengl32.lib")
        #pragma comment(lib, "assimp.lib")
    #else
        #pragma comment(lib, "sfml-main.lib")
        #pragma comment(lib, "sfml-graphics.lib")
        #pragma comment(lib, "sfml-system.lib")
        #pragma comment(lib, "sfml-window.lib")
        #pragma comment(lib, "glew32.lib")
        #pragma comment(lib, "glu32.lib")
        #pragma comment(lib, "opengl32.lib")
        #pragma comment(lib, "assimp.lib")
    #endif
#endif

aiScene* loadMesh(const std::string& filename)
{
    Assimp::Importer importer;

    aiScene* scene = (aiScene*) importer.ReadFile(filename, aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType);

    if (!scene)
    {
        fprintf(stderr, "Error during mesh loading: %s\n", importer.GetErrorString());
        return 0;
    }

    return scene;
}

int main()
{
    // create the window (remember: it's safer to create it in the main thread due to OS limitations)
    sf::Window window(sf::VideoMode(800, 600), "OpenGL");

    GLenum res = glewInit();

    if (res != GLEW_OK)
    {
        fprintf(stderr, "GLEW Init error: '%s'\n", glewGetErrorString(res));
        return 1;
    }

    // activate the window's context
    window.setActive(true);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);

    // Setup a perspective projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90.f, 1.f, 1.f, 500.f);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    // setup some primitive camera!
    sf::Vector3f eye(0, 10.0, -5.0), lookat(0, 0, 0), up(0, 1, 0);

    // Cube!
    GLfloat vertices[] = {
        0.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,

        0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        0.0f, 1.0f, 1.0f,

        0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 1.0f,
        0.0f, 1.0f, 0.0f,

        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 0.0f,

        0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 0.0f,

        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 0.0f
    };

    GLuint indices[] = {
        0, 1, 2, 3,
        4, 5, 6, 7,
        8, 9, 10, 11,
        12, 13, 14, 15,
        16, 17, 18, 19,
        20, 21, 22, 23
    };

    GLuint verticesVBO, indicesVBO;

    glGenBuffers(1, &verticesVBO);
    glBindBuffer(GL_ARRAY_BUFFER, verticesVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &indicesVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesVBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * sizeof(indices), indices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, verticesVBO);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, sizeof(GLfloat) * 3, (void*) (0));   //The starting point of the VBO, for the vertices

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    float alpha = 0.0f;

    // the rendering loop
    while (window.isOpen())
    {
        sf::Event event;

        // while there are pending events...
        while (window.pollEvent(event))
        {
            // check the type of the event...
            switch (event.type)
            {
                // window closed
                case sf::Event::Closed:
                    window.close();

                    glDeleteBuffers(1, &verticesVBO);
                    glDeleteBuffers(1, &indicesVBO);
                break;

                // key pressed
                case sf::Event::KeyPressed:
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
                        eye.z += 1.0;

                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
                        eye.z -= 1.0;

                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
                        eye.x -= 1.0;

                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
                        eye.x += 1.0;
                break;

                case sf::Event::Resized:
                printf("Resized!\n");
                    glViewport(0, 0, (GLsizei) event.size.width, (GLsizei) event.size.height);

                    glMatrixMode(GL_PROJECTION);
                    glLoadIdentity();
                    gluPerspective(45.0, event.size.width / (GLdouble) event.size.height, 0.1, 10000.0);

                    glMatrixMode(GL_MODELVIEW);
                    glLoadIdentity();
                break;

                // we don't process other types of events
                default:
                break;
            }
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glLoadIdentity();

        glPushMatrix();
            gluLookAt(eye.x, eye.y, eye.z, lookat.x, lookat.y, lookat.z, up.x, up.y, up.z);

            glPushMatrix();
                glRotatef(alpha, 0.0, 1.0, 0.0);
                glColor3f(0.5f, 0.5f, 0.0f);

                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesVBO);
                glDrawElements(GL_QUADS, sizeof(indices), GL_UNSIGNED_INT, (void*) (0));   //The starting point of the IBO
            glPopMatrix();
        glPopMatrix();

        alpha += 10.0f;

        window.display();
    }

    return 0;
}
