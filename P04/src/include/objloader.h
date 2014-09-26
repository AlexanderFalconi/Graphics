#ifndef __OBJLOADER_H__
#define __OBJLOADER_H__
using std::vector;

bool load_obj(const char* path, vector<glm::vec3> &out_vertices, vector<glm::vec2> &out_uvs, vector<glm::vec3> &out_normals);

#endif