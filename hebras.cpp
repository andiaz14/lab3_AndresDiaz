#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <iostream>
#include <sstream>

using namespace std;

void lectorArchivoSecuencial(char archivo)
{
  char c[2];
  string line;
  int lineCount = 0;
  string word;
  int wordCount = 0;
  char letter;
  int charCount = 0;


  int stream  = open(archivo, O_RDONLY);

  if(stream == -1)
  {
    std::cout << "Error al abrir archivo" << std::endl;
  }

  while(read(stream,c,1))
  {
    if(c[0] == '\n')
    {
      lineCount++;
      wordCount++:
    }
    else if(c[0] == ' ')
    {
      wordCount++;
    }

    charCount++;
  }
  close (stream);

}

/*void *buscarLetras (void *param) {
  char *str;
  char c[2];
  int cnt = 0;

  str = (char *) param;

  int stream = open ("filename.txt", O_RDONLY);

  if (stream == -1) {
    std::cout << "Error al abrir archivo" << std::endl;
    pthread_exit(0);
  }

  while (read(stream,c,1)) {
    if (strchr(str,c[0])) {
      cnt++;
      std::string t = "Encontró: " + (std::string ) str + "\n";
      std::cout << t;
      sleep(1);
    }
  }
  close (stream);
  
  // Para convertir un número en string.
  std::ostringstream oss;
  oss << cnt;
  std::string t = "Caracter: " + (std::string ) str + "[" + oss.str() + " ocurencias]" + "\n";
  std::cout << t;

  pthread_exit(0);
}*/

int main(int argc, char *argv[]) {
  /*
  pthread_t threads[argc - 1];
  int i = 0;

  //Crea todos los hilos 
  for (i=0; i < argc - 1; i++) {
    pthread_create(&threads[i], NULL, buscarLetras, argv[i+1]);
  }

  //Para esperar por el término de todos los hilos 
  for (i=0; i< argc - 1; i++) {
    pthread_join(threads[i], NULL);
  }
  */
  for(int i = 0; i < argc -1; i++)
  {
    lectorArchivoSecuencial(argv[i+1]);
  }


  return 0;
}