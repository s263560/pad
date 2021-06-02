using System;
using System.Text.RegularExpressions;
using System.Diagnostics;
using System.IO;
using System.Collections.Generic;
using System.Threading.Tasks;
using System.Threading;

// exerc_grep caminho padrao
// - pesquisa pelo padrão de expressão regular 'padrao' em todos os arquivos da pasta 'caminho'

namespace exerc_grep {
    class Program {

        static public Regex r;

        static void Main(string[] args) {
            Stopwatch sw;                      // cronômetro para tempo de execução
            Mutex mutex = new Mutex();         // mutex para adicionar item à lista

            r = new Regex(args[1], RegexOptions.IgnoreCase);      // cria (compila) expressão regular
            string[] fileEntries = Directory.GetFiles(args[0]);   // pega lista de arquivos do diretório

            Console.WriteLine("Pesquisando expressão '" + args[1] + "' nos arquivos em\n" + args[0]);
            Console.WriteLine("Número total de arquivos: " + fileEntries.Length);

            // Primeiro processamento - faz leitura de todos os arquivos apenas para carregar arquivos em caches
            // -- como na primeira leitura os arquivos podem caber todos em cache na memória, lê todos os arquivos
            // -- de modo que a medição de tempo serial e paralelo ocorram nas mesmas condições
            sw = Stopwatch.StartNew();
            foreach (var f in fileEntries) {
                string texto = File.ReadAllText(f, System.Text.Encoding.ASCII);
            }
            sw.Stop();
            Console.WriteLine("Pré-processamento: {0}ms", sw.ElapsedMilliseconds);


            // realiza pesquisa no conteúdo dos arquivos de forma sequencial
            sw = Stopwatch.StartNew();
            int s_NumArqs = 0;
            var s_achados = new List<string>();     // lista para armazenar nomes dos arquivos encontrados
            foreach (var f in fileEntries) {
                string texto = File.ReadAllText(f, System.Text.Encoding.ASCII);    // lê arquivo usando codificação ASCII
                if (r.IsMatch(texto)) {             // caso encontre o padrão da expressão regular no arquivo,
                    s_NumArqs += 1;                 // incrementa contador
                    s_achados.Add(f);               // adiciona à lista de arquivos encontrados
                }
            }
            sw.Stop();
            Console.WriteLine("Serial: {0}ms", sw.ElapsedMilliseconds);
            Console.WriteLine("Número de arquivos encontrados (serial): {0}", s_NumArqs);


            // realiza mesma pesquisa de forma paralela
            sw = Stopwatch.StartNew();
            int p_NumArqs = 0;
            var p_achados = new List<string>();
            var opt = new ParallelOptions() {     // variável com opções de paralelismo
                MaxDegreeOfParallelism = 4        // Opção para número máximo de threads
            };
            Parallel.ForEach(fileEntries, opt, f => {
                string texto = File.ReadAllText(f, System.Text.Encoding.ASCII);    // lê arquivo usando codificação ASCII
                if (r.IsMatch(texto)) {                     // caso encontre o padrão da expressão regular no arquivo,
                    Interlocked.Increment(ref p_NumArqs);   // Operação de incremento de forma atômica
                    mutex.WaitOne();                        // mutex para executar método na lista p_achados
                    p_achados.Add(f);
                    mutex.ReleaseMutex();
                }
            });
            sw.Stop();
            Console.WriteLine("Paralelo: {0}ms", sw.ElapsedMilliseconds);
            Console.WriteLine("Número de arquivos encontrados (serial): {0}", s_NumArqs);

            // Apresenta arquivos onde encontrou o padrão
            Console.WriteLine("Padrão '" + args[1] + "' encontrado nos seguintes arquivos:" + args[0]);
            foreach (var t in p_achados)
                Console.WriteLine(t);
        }
    }
}
