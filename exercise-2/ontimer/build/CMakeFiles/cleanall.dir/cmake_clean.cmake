FILE(REMOVE_RECURSE
  "CMakeFiles/cleanall"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/cleanall.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
