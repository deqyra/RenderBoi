#ifndef RENDERBOI__MAIN_FUNCTIONS_HPP
#define RENDERBOI__MAIN_FUNCTIONS_HPP

#include <string>
#include <vector>

#include <renderboi/examples/gl_sandbox.hpp>

#include "renderboi_parameters.hpp"

// Halt the execution with a clean exit
int abortWithError(std::string message, bool terminateBackend = true);

// Instantiate all available sandboxes
std::vector<Renderboi::GLSandbox*> createAllSandboxes();

// Process command line arguments and parse parameters
bool processArguments(int argc, char** argv, RenderboiParameters& dest);

// Print usage message and exit cleanly
void printHelp();

#endif//RENDERBOI__MAIN_FUNCTIONS_HPP