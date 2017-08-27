/*
 * Copyright (c) 2017 Bennet Carstensen
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <soEngine.hpp>

#include <utils/err/soException.hpp>

int
main()
{
  try
  {
    try
    {
      so::Engine engine;

      GLFWwindow* window(engine.getWindow());

      vkDeviceWaitIdle(engine.getVkDevice());

      while(!glfwWindowShouldClose(window))
      {
        glfwPollEvents();

        engine.updateUniformBuffer();

        engine.drawFrame();

      }

      vkDeviceWaitIdle(engine.getVkDevice());
    }
    catch(...)
    {
      std::throw_with_nested(so::utils::err::Exception<std::runtime_error>
         ("Unexpected error:", PRETTY_FUNCTION_SIG));
    }
  }
  catch(so::utils::err::Exception<std::runtime_error> const& e)
  {
    e.print();

    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}