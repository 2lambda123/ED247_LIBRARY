###############################################################################
# The MIT Licence                                                             #
#                                                                             #
# Copyright (c) 2021 Airbus Operations S.A.S                                  #
#                                                                             #
# Permission is hereby granted, free of charge, to any person obtaining a     #
# copy of this software and associated documentation files (the "Software"),  #
# to deal in the Software without restriction, including without limitation   #
# the rights to use, copy, modify, merge, publish, distribute, sublicense,    #
# and/or sell copies of the Software, and to permit persons to whom the       #
# Software is furnished to do so, subject to the following conditions:        #
#                                                                             #
# The above copyright notice and this permission notice shall be included     #
# in all copies or substantial portions of the Software.                      #
#                                                                             #
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR  #
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,    #
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE #
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER      #
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING     #
# FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER         #
# DEALINGS IN THE SOFTWARE.                                                   #
###############################################################################
function(add_subdirectory_with_rpath _folder_name)
  add_subdirectory(${_folder_name})
  set_target_properties(${_folder_name} PROPERTIES INSTALL_RPATH "$ORIGIN/../../../lib")
endfunction()

add_subdirectory_with_rpath(walk_configuration)
add_subdirectory_with_rpath(exchange_stream_wait_frame)
add_subdirectory_with_rpath(exchange_stream_event)
add_subdirectory_with_rpath(exchange_stream_wait_during)
add_subdirectory_with_rpath(exchange_signal_wait_frame)

# Custum target to compile only the samples and there dependencies
add_custom_target(samples
    DEPENDS
        walk_configuration
        exchange_stream_wait_frame
        exchange_stream_event
        exchange_stream_wait_during
        exchange_signal_wait_frame
)

