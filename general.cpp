/*
 *  Written by    : Nikoletta Barmpa, Andreas Evangelatos
 */

#include <cstdlib>
#include <string>
#include "general.hpp"
#include "error.hpp"
#include <set>
#include "lexer_funcs.hpp"
#include "ast.hpp"
#include "llvmhead.hpp"
#include <cstdlib>

#include <iostream>

const char * filename;
int linecount;
bool optimize_flag = false;
bool _f_flag = false;
bool _i_flag = false;
std::string interMediateOutFileName;
std::string assemblyOutFileName;

llvm::TargetMachine* TheTargetMachine;
extern std::set<std::string> fileset;
extern std::string currentFilename;




#define BOLD  "\e[1m"
#define reset "\e[0m"
void printHelpMessage(){
	fprintf(stderr, "Usage: " BOLD "./ggec [-O] *.eds\n" reset);
	fprintf(stderr, "       \t\tProducing intermediate code in *.imm and final code in *.asm\n\n");
	fprintf(stderr, "       " BOLD "./ggec [-O] -f\n" reset);
	fprintf(stderr, "       \t\tExpecting input program in stdin and will produce final code in stdout\n\n");
	fprintf(stderr, "       " BOLD "./ggec [-O] -i\n" reset);
	fprintf(stderr, "       \t\tExpecting input program in stdin and will produce immediate code in stdout\n\n");
   fprintf(stderr, "       " BOLD "./ggec --help\n" reset);
	fprintf(stderr, "       \t\tPrint this message\n\n");
}

bool hasEnding (std::string const &fullString, std::string const &ending) {
    if (fullString.length() >= ending.length()) {
        return (0 == fullString.compare (fullString.length() - ending.length(), ending.length(), ending));
    } else {
        return false;
    }
}

void handleArguments(int argc, char** argv)
{
   if(argc <= 1){
      printHelpMessage();
      exit(1);
   }

   for(int i = 1; i < argc; i++){
      if (argv[i] == std::string("-O")){
         optimize_flag = true;
      }
      else if(argv[i] == std::string("-f") && !_i_flag && fileset.empty()){
         _f_flag = true;
      }
      else if(argv[i] == std::string("-i") && !_f_flag && fileset.empty()){
         _i_flag = true;
      }
      else if (argv[i] == std::string("--help")){
         printHelpMessage();
         exit(0);
      }
      else if (hasEnding(std::string(argv[i]), std::string(".eds")) && (!_i_flag) && (!_f_flag)){
         yypop_buffer_state();
         yyin = fopen(argv[i], "r");
         if(! yyin){
	      	fprintf(stderr, "Failed to open input file\n");
		      exit(1);
	      }
         fileset = { argv[i] };
         currentFilename = fileset.find(argv[i])->c_str();
         yypush_buffer_state(yy_create_buffer( yyin, YY_BUF_SIZE ));

         interMediateOutFileName = currentFilename;
         interMediateOutFileName.replace(interMediateOutFileName.find(std::string(".eds")), sizeof(".eds")-1, std::string(".imm"));
         assemblyOutFileName = currentFilename;
         assemblyOutFileName.replace(assemblyOutFileName.find(std::string(".eds")), sizeof(".eds")-1, std::string(".asm"));
      }
      else if (!fileset.empty() || _i_flag || _f_flag){
         fprintf(stderr, "Can handle only one of -f, -i, *.eds\n\n");
         printHelpMessage();
         exit(1);
      }
      else{
         printHelpMessage();
         exit(1);
      }

   }
   if(!_f_flag && !_i_flag && fileset.empty()){
      fprintf(stderr, "Expected One of -f, -i, *.eds\n\n");
      printHelpMessage();
      exit(1);

   }

   if(_f_flag || _i_flag){
      // this is not wrong because we only allow *.eds files
      currentFilename = "stdin";
      
      //should never use this.
      interMediateOutFileName = "";
      assemblyOutFileName = "";
      
      fileset = { currentFilename };
   }


   // see Kaleidoscope Ch.8 
   // https://llvm.org/docs/tutorial/MyFirstLanguageFrontend/LangImpl08.html

   llvm::InitializeAllTargetInfos();
   llvm::InitializeAllTargets();
   llvm::InitializeAllTargetMCs();
   llvm::InitializeAllAsmParsers();
   llvm::InitializeAllAsmPrinters();
   auto TargetTriple = llvm::sys::getDefaultTargetTriple();
   AST::TheModule->setTargetTriple(TargetTriple);

   std::string Error;
   auto Target = llvm::TargetRegistry::lookupTarget(TargetTriple, Error);

   // Print an error and exit if we couldn't find the requested target.
   // This generally occurs if we've forgotten to initialise the
   // TargetRegistry or we have a bogus target triple.
   if (!Target) {
      llvm::errs() << Error;
      exit(1);
   }

   // We don't use any additional features, options or relocation model.
   auto CPU = "generic";
   auto Features = "";

   llvm::TargetOptions opt;
   auto RM = llvm::Optional<llvm::Reloc::Model>();
   TheTargetMachine = Target->createTargetMachine(TargetTriple, CPU, Features, opt, RM);

   AST::TheModule->setDataLayout(TheTargetMachine->createDataLayout());

}

void handleOutput()
{
   // Only intermediate code in stdout
   if(_i_flag){
      AST::TheModule->print(llvm::outs(), nullptr);
      return;
   }

   //====================================================================================//
   // Create the Pass for printing                                                       //
   //====================================================================================//
   std::error_code EC;
   llvm::legacy::PassManager pass;
   auto FileType = llvm::CGFT_AssemblyFile;

   if(_f_flag){      
      llvm::raw_fd_ostream outDest("-", EC);
      // llvm::raw_fd_ostream outDest(STDOUT_FILENO, false);

      if (TheTargetMachine->addPassesToEmitFile(pass, outDest, nullptr, FileType)) {
         llvm::errs() << "TargetMachine can't emit a file of this type";
         exit(1);
      }

      pass.run(*AST::TheModule);
      outDest.flush();
      return;
   }

   // Here is the *.eds case need to print both .imm file and .asm file

   //====================================================================================//
   // Printing Intermediate Representation                                               //
   //====================================================================================//
   
   
   llvm::raw_fd_ostream interDest(interMediateOutFileName, EC, llvm::sys::fs::OF_None);
   
   if (EC) {
      llvm::errs() << "Could not open file: " << EC.message();
      exit(1);
   }
   AST::TheModule->print(interDest, nullptr);
   interDest.flush();

   //====================================================================================//
   // Printing Assembly                                                                  //
   //====================================================================================//
   

   // llvm::raw_fd_ostream assemblyDest(assemblyOutFileName, EC, llvm::sys::fs::OF_None);
   llvm::raw_fd_ostream assemblyDest(assemblyOutFileName, EC);
   llvm::legacy::PassManager pass2;
   FileType = llvm::CGFT_AssemblyFile;
   if (EC) {
      llvm::errs() << "Could not open file: " << EC.message();
      exit(1);
   }

   if (TheTargetMachine->addPassesToEmitFile(pass2, assemblyDest, nullptr, FileType)) {
      llvm::errs() << "TargetMachine can't emit a file of this type";
      exit(1);
   }

   pass2.run(*AST::TheModule);
   assemblyDest.flush();
   
}