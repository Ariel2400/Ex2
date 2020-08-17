#include "ErrorCode.h"

extern "C" {
bool error_isSuccess(ErrorCode code) {
  if (code == ERROR_SUCCESS) {
    return true;
  }
  return false;
}
const char *error_getErrorMessage(ErrorCode code) {
  switch (code) {
  case ERROR_SUCCESS:
    return "Success!";
  case ERROR_FAILURE:
    return "Failure!";
  case ERROR_FAILURE_CANT_ALLOCATE:
    return "Can't allocate!";
  case ERROR_FAILURE_INPUT_ERROR:
    return "Wrong input!";
  default:
    return "";
  }
}
}