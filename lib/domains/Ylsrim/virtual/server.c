/*    /domains/Ylsrim/virtual/server.c
 *    from the Dead Souls V Object Library
 *    created by Descartes of Borg 960302
 */

//#define __DIR__ "/your/dir/here/"
#define __DIR__ "/domains/Ylsrim/virtual/"

mixed compile_object(string file) {
    string *path;
    object ob;
    int x, y;

    //if( previous_object() != master() ) return 0;
    path = explode(file, "/");
    //if( sizeof(path) != 5 ) return "Wrong size path";
    if( file_size(__DIR__ + path[3] + ".c") < 1 ) return "bad file";
    if( sscanf(path[4], "%d,%d", x, y) != 2 ) return "missing comma";    
    if( !(ob = new(__DIR__ + path[3], x, y)) ) return "failed to compile";
    return ob;
}
