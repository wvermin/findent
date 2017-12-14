#!/bin/bash
sed 's/\\/\\\\/g;s/"/\\"/g;s/^/  std::cout << "/;s/$/" << std::endl;/'
