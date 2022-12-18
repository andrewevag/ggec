
void writeInteger_int (int n);
void writeBoolean_bool (bool b);
void writeChar_char (char c);
void writeReal_double (double d);
void writeString_char_ptr (char * s);

int readInteger ();
bool readBoolean ();
char readChar ();
double readReal ();
void readString_int_char_ptr (int size, char * s);