/*
 * 3D RAD V7.22 PROJECT DECOMPILER
 * hacky wachy tool to reverse the decompilation done by 3D Rad's compilation process
 * Usefull if you want to recover projects you wrote before in 3D Rad
 */

/* 
 * NOTES AND REMAKRS:
 * 1.) Commented out code was for checking the differences between a compiled project and a normal project.
 * 2.) 
 */
//#define CHECKF

using System;
using System.Windows.Forms;
using System.IO;

namespace _3DRadProjectDecompiler
{
    class Program
    {
        [STAThread]
        static void Main(string[] args)
        {
#if CHECKF
        CompareFiles();
#else
            OpenFileDialog ofn = new OpenFileDialog()
            {
                Multiselect = false,
                Title = "Select the project you wish to get decompiled",
                InitialDirectory = "C:\\"
            };
            while(ofn.ShowDialog() != DialogResult.OK) { }
            DecompileProject(ofn.FileName);
            ofn.Dispose();
#endif
            Console.WriteLine("[FINISH]Decompiled project.");
            Console.ReadLine(); //PAUSE
        }
        static void CompareFiles()
        {
            string p1, p2;
            byte[] Fstr1;
            byte[] Fstr2;
            OpenFileDialog ofn = new OpenFileDialog()
            {
                Multiselect = false,
                Title = "Select the first project",
            };
            while(ofn.ShowDialog() != DialogResult.OK) { }
            p1 = ofn.FileName;
            Fstr1 = File.ReadAllBytes(p1);
            Console.WriteLine("[OK]Selected file 1:" + p1);
            Console.WriteLine("[OK]Selected file 1:" + p1);
            
            ofn.Title = "Select the second project";
            while(ofn.ShowDialog() != DialogResult.OK) { }

            p2 = ofn.FileName;
            Fstr2 = File.ReadAllBytes(p2);
            Console.WriteLine("[OK]Selected file 2:" + p2);
            
            ofn.Dispose(); //tfw C++ OpenFileDialog doesn't make memory leaks iirc
            byte a;
            
            if(Fstr1.Length == Fstr2.Length)
            {
                Console.WriteLine("[CHCEK1] The files have the same size.");
                a = 0;
            }
            else if(Fstr1.Length > Fstr2.Length)
            {
                Console.WriteLine("[CHECK1] File 1 > File 2");
                a = 1;
            }
            else
            {
                Console.WriteLine("[CHECK1] File 2 > File 1");
                a = 2;
            }

            int l;
            
            switch(a)
            {
                case 2:
                    l = Fstr2.Length;
                    break;
                default:
                    l = Fstr1.Length;
                    break;
            }
            
            for(int i =0; i < l;i++)
            {
                if(Fstr1[i] != Fstr2[i])
                {
                    Console.WriteLine("[RESULTS] Found " + i + " with values: ");
                    Console.WriteLine("1 = (INT)" + Fstr1[i] + " (CHAR) " + (char)Fstr1[i]);
                    Console.WriteLine("2 = (INT)" + Fstr2[i] + " (CHAR) " + (char)Fstr2[i]);
                }
            }

        }

        static void DecompileProject(string filename)
        {
            byte[] file = File.ReadAllBytes(filename);

            //revert the dirty bits.
            file[2] = 0; //possible 'compilation' flag

            //these are unknown. Possible int value making a SHA value????. If true needs to get calculated.
            //file[8] = 196;
            //file[9] = 115;
            //file[10] = 188;
            //file[11] = 170;

            File.WriteAllBytes(Path.GetDirectoryName(filename)+"\\"+Path.GetFileNameWithoutExtension(filename) + "_decompiled.3dr", file);
        }
    }
}
