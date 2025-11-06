module;

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <array>
#include <string_view>

export module Shader;

export namespace Graphics {
    class Shader {
    public:
        unsigned int ID;

        Shader(const char* vertexPath, const char* fragmentPath) {
            auto [vertexResult, vertexCode] = readFile(vertexPath);
            if (!vertexResult) {
                throw std::runtime_error("Failed to read vertex shader file: " + std::string(vertexPath));
            }

            auto [fragmentResult, fragmentCode] = readFile(fragmentPath);
            if (!fragmentResult) {
                throw std::runtime_error("Failed to read fragment shader file: " + std::string(fragmentPath));
            }

            const char* vShaderCode = vertexCode.c_str();
            const char* fShaderCode = fragmentCode.c_str();

            const unsigned int vertex = [&vShaderCode] {
                const unsigned int id = glCreateShader(GL_VERTEX_SHADER);
                glShaderSource(id, 1, &vShaderCode, nullptr);
                glCompileShader(id);
                return id;
            }();
            checkCompileErrors(vertex, "VERTEX");

            const unsigned int fragment = [&fShaderCode] {
                const unsigned int id = glCreateShader(GL_FRAGMENT_SHADER);
                glShaderSource(id, 1, &fShaderCode, nullptr);
                glCompileShader(id);
                return id;
            }();
            checkCompileErrors(fragment, "FRAGMENT");

            ID = [&vertex, &fragment] {
                const unsigned int program = glCreateProgram();
                glAttachShader(program, vertex);
                glAttachShader(program, fragment);
                glLinkProgram(program);
                return program;
            }();
            checkCompileErrors(ID, "PROGRAM");

            glDeleteShader(vertex);
            glDeleteShader(fragment);
        }

        void use() const noexcept { glUseProgram(ID); }

        void setBool(std::string_view name, bool value) const {
            glUniform1i(glGetUniformLocation(ID, name.data()), static_cast<int>(value));
        }

        void setInt(std::string_view name, int value) const {
            glUniform1i(glGetUniformLocation(ID, name.data()), value);
        }

        void setFloat(std::string_view name, float value) const {
            glUniform1f(glGetUniformLocation(ID, name.data()), value);
        }

        void setVec3(std::string_view name, const glm::vec3& value) const {
            glUniform3fv(glGetUniformLocation(ID, name.data()), 1, glm::value_ptr(value));
        }

        void setMat4(std::string_view name, const glm::mat4& mat) const {
            glUniformMatrix4fv(glGetUniformLocation(ID, name.data()), 1, GL_FALSE, glm::value_ptr(mat));
        }

    private:
        [[nodiscard]]
        auto readFile(const char* path) const -> std::pair<bool, std::string> {
            std::ifstream file(path);
            if (!file.is_open()) {
                return {false, {}};
            }

            std::string content;
            content.assign(std::istreambuf_iterator<char>(file),
                          std::istreambuf_iterator<char>());
            file.close();

            return {true, std::move(content)};
        }

        void checkCompileErrors(unsigned int shader, std::string_view type) const {
            int success = 0;
            std::array<char, 1024> infoLog{};

            if (type != "PROGRAM") {
                glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
                if (success == GL_FALSE) {
                    glGetShaderInfoLog(shader, static_cast<GLsizei>(infoLog.size()), nullptr, infoLog.data());
                    std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog.data() << std::endl;
                }
            } else {
                glGetProgramiv(shader, GL_LINK_STATUS, &success);
                if (success == GL_FALSE) {
                    glGetProgramInfoLog(shader, static_cast<GLsizei>(infoLog.size()), nullptr, infoLog.data());
                    std::cout << "ERROR::PROGRAM_LINKING_ERROR\n" << infoLog.data() << std::endl;
                }
            }
        }
    };
}
