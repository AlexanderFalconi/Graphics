#ifndef OBJLOADER_H
#define OBJLOADER_H

void load_obj(const char* filename, vector<glm::vec4> &vertices, vector<glm::vec3> &normals, vector<GLushort> &elements);

#endif