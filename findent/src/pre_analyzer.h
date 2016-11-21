#include <stack>
class pre_analyzer
{
   std::stack<bool> ifelse_stack; // to note if there is an #else after #if

   public:
   int analyze(const std::string s);
   enum {NONE_pre,IF_pre,ELIF_pre,ELSE_pre,ENDIF_pre,ENDIFE_pre};


};
