extern char *parser_result;


extern int yyparse();

int main()
{
   if(yyparse()==0) {
      printf("Parse successful!\n Expression Tree Version 2: ");
      expr_print(parser_result);
      printf("\n");
   } else {
      printf("Parse failed.\n");
   }
}
