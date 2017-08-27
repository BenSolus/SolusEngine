/*
 * Copyright (c) 2017 Bennet Carstensen
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <vk/soVkMesh.hpp>

#include <utils/err/soException.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <unordered_map>

so::vk::Mesh::Mesh() {}

so::vk::Mesh::Mesh(std::string const path)
{
  Assimp::Importer importer;

  importer.SetPropertyInteger(AI_CONFIG_PP_PTV_NORMALIZE, true);

  const aiScene* scene(importer.ReadFile(path,
                                         aiProcess_JoinIdenticalVertices |
                                         aiProcess_Triangulate |
                                         aiProcess_PreTransformVertices |
                                         aiProcess_ImproveCacheLocality |
                                         aiProcess_FlipWindingOrder));

  if(!scene)
    throw utils::err::Exception<std::runtime_error>(importer.GetErrorString(),
                                                    PRETTY_FUNCTION_SIG);
  else
  {
    aiMesh **meshes(scene->mMeshes);

    for(uint32_t m(0); m < scene->mNumMeshes; ++m)
    {
      const aiVector3D* vertices(meshes[m]->mVertices);

      for (uint32_t v(0); v < meshes[m]->mNumVertices; ++v)
      {
        mVertices.push_back
          (Vertex({ vertices[v].x, -vertices[v].y, vertices[v].z },
                  { 1.0f, 1.0f, 1.0f },
                  {  meshes[m]->mTextureCoords[0][v].x,
                     1.0f - meshes[m]->mTextureCoords[0][v].y } ));
      }
    }

    for(uint32_t m(0); m < scene->mNumMeshes; ++m)
    {
      const aiFace* faces(meshes[m]->mFaces);

      const uint32_t oldNumIndices(static_cast<uint32_t>(mIndices.size()));

      for(uint32_t f(0); f < meshes[m]->mNumFaces; ++f)
        for(uint32_t i(0); i < 3; ++i)
          mIndices.push_back(oldNumIndices + faces[f].mIndices[i]);
    }
  }
}

std::vector<so::vk::Vertex> so::vk::Mesh::mVertices =
  std::vector<so::vk::Vertex>();

std::vector<uint32_t> so::vk::Mesh::mIndices  = std::vector<uint32_t>();