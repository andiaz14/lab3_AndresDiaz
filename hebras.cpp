#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <iostream>
#include <sstream>
#include <chrono>

using namespace std;

void lectorArchivoSecuencial(char* archivo)
{
  char c[2];
  string line;
  int lineCount = 0;
  string word;
  int wordCount = 0;
  string letter;
  int charCount = 0;


  int stream  = open(archivo, O_RDONLY);

  if(stream == -1)
  {
    std::cout << "Error al abrir archivo" << std::endl;
    return;
  }

  while(read(stream,c,1))
  {
    if(c[0] == '\n')
    {
      lineCount++;
      wordCount++;
    }
    else if(c[0] == ' ')
    {
      wordCount++;
    }
    else if((c[0] >= 'a' && c[0] <= 'z')||(c[0]>='A' && c[0]<='Z'))
    {
      charCount++;
    }
  }
  close (stream);

  std::ostringstream oss;
  oss << lineCount;
  line = oss.str();
  std::ostringstream oss1;
  oss1 << wordCount;
  word = oss1.str();
  std::ostringstream oss2;
  oss2 << charCount;
  letter = oss2.str();


  string str = "lineas: " + line + ", palabras: " + word + ", caracteres: " + letter  + "\n";

  std::cout << "texto: " << archivo << ", " << str;

}

void *lectorArchivoThread (void *param) {
  char *archivo;
  char c[2];
  string line;
  int lineCount = 0;
  string word;
  int wordCount = 0;
  string letter;
  int charCount = 0;

  archivo = (char *) param;

  int stream = open (archivo, O_RDONLY);

  if (stream == -1) {
    std::cout << "Error al abrir archivo" << std::endl;
    pthread_exit(0);
  }

  while (read(stream,c,1)) {
    if(c[0] == '\n')
    {
      lineCount++;
      wordCount++;
    }
    else if(c[0] == ' ')
    {
      wordCount++;
    }
    else if((c[0] >= 'a' && c[0] <= 'z')||(c[0]>='A' && c[0]<='Z'))
    {
      charCount++;
    }
  }
  close (stream);
  
  std::ostringstream oss;
  oss << lineCount;
  line = oss.str();
  std::ostringstream oss1;
  oss1 << wordCount;
  word = oss1.str();
  std::ostringstream oss2;
  oss2 << charCount;
  letter = oss2.str();


  string str = "lineas: " + line + ", palabras: " + word + ", caracteres: " + letter  + "\n";

  std::cout << "texto: " << archivo << ", " << str;

  pthread_exit(0);
}


int main(int argc, char *argv[]) {

  std::cout << "Secuencial" << '\n';
  auto start = chrono::system_clock::now();

  for(int i = 0; i < argc -1; i++)
  {
    lectorArchivoSecuencial(argv[i+1]);
  }

  auto end = chrono::system_clock::now();
  chrono::duration<double> segundos = end - start;

  std::cout << "segundos: " << segundos.count() << '\n';
  std::cout << "Por hebras." << '\n';
  start = chrono::system_clock::now();
  pthread_t threads[argc - 1];
  int i = 0;
  
  //Crea todos los hilos 
  for (i=0; i < argc - 1; i++) {
    pthread_create(&threads[i], NULL, lectorArchivoThread, argv[i+1]);
  }


  //Para esperar por el término de todos los hilos 
  for (i=0; i< argc - 1; i++) {
    pthread_join(threads[i], NULL);
  }
  end = chrono::system_clock::now();
  
  segundos = end - start;
  std::cout << "segundos: " << segundos.count() << '\n';

  return 0;
}