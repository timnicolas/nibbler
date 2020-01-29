// -- Exceptions errors --------------------------------------------------------
Parser::DynGuiManagerException::DynGuiManagerException()
: std::runtime_error( "Parser Exception" ) {}

Parser::DynGuiManagerException::DynGuiManagerException( const char* what_arg )
: std::runtime_error( std::string(std::string("ParserError: ") + what_arg).c_str() ) {}