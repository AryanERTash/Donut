
/*
 * A implementation of donut.c in java by @AryanERTash
 * This implementation is highly inefficient as it does a lot of memeory allocations, i made it only for fun
 * Also there is some bug :|
 */
import org.ejml.simple.SimpleMatrix;

import static java.lang.Math.cos;
import static java.lang.Math.sin;   //static import of maths functions
import static java.lang.Math.PI;

import java.lang.Math;


public class Donut {
   private static final double thetaSpacing = 0.07;// a circle
   private static final double phiSpacing = 0.02; // moves circle along y axis

   private static final double ASpacing = 0.02; // sampling in x axis rotation
   private static final double BSpacing = 0.01; // smapling of z axis rotation

   
   private static final int centerRadius=10;
   private static final int ringRadius=4;
   
   
   private static final int WIDTH=2*(centerRadius+ringRadius)+10;    // added 10 to incraese buffer to avoid inder out of bound error
   private static final int HEIGHT = WIDTH;
   private static final int SHIFT = centerRadius+ringRadius;
   
   private static char zBuffer[][] = new char[HEIGHT][WIDTH];
   private static String CHARACTERS = ".,-~:;=!*#$@";
   
   private static SimpleMatrix L = new SimpleMatrix(new double[][] {
      {0, 1, -1}  
   });

   public static void setZBuf(int x, int y, char value) {
      // this helps to map our buffer as x and y axis
      // shifts everything in the center
      x+=SHIFT;
      y+=SHIFT;
      y=HEIGHT-y-1;
      zBuffer[y][x] = value;
   }
   
   
   public static char getZVal(int x, int y) {
      x+=SHIFT;
      y+=SHIFT;
      y=HEIGHT-y-1;
      return zBuffer[y][x];
   }

   public static int round(double x) {
      return (int)(x+0.5);
   }
   public static void clearScreen()  {  
      System.out.print("\033[H\033[2J"); 
      System.out.flush();  
  } 

  public static void clearBuffer() {
   for(int i=0;i<zBuffer.length;++i) {
      for(int j=0;j<zBuffer[i].length;++j) {
         zBuffer[i][j]=0;
      }
   }
}


   public static void main(String[] args) throws InterruptedException {
      
      double A=0; // x rotation angle
      double B=0; // z rotation angle

      while (true) {
         
         
         for(double phi=0;phi<2*PI;phi+=phiSpacing) {
            
            for(double theta =0;theta<2*PI;theta+=thetaSpacing) {
               double X = centerRadius + ringRadius*cos(theta);
               double Y = ringRadius*sin(theta);
               double Z=0;

                
               SimpleMatrix yRotationM = new SimpleMatrix(new double[][] {
                  {cos(phi), 0, sin(phi)},
                  {0,         1     , 0},
                  {-sin(phi), 0, cos(phi)}
               });

               SimpleMatrix xRotationM = new SimpleMatrix(new double[][] {
                  {1,      0,    0},
                  {0, cos(A), -sin(A)},
                  {0, sin(A), cos(A)}

               });

               SimpleMatrix zRotationM = new SimpleMatrix(new double[][] {
                  {cos(B), -sin(B), 0},
                  {sin(B), cos(B), 0},
                  {0,      0,    1}
               });

               SimpleMatrix coords = new SimpleMatrix(new double[][]{
                  {X},
                  {Y},
                  {Z}
               }); 

               SimpleMatrix normalvec = new SimpleMatrix(new double[][] {
                  {cos(theta)},
                  {sin(theta)},
                  {0}
               });

               SimpleMatrix result = zRotationM.mult(xRotationM.mult(yRotationM.mult(coords)));
               X=result.get(0,0);
               Y=result.get(1,0);
               Z=result.get(2,0);
               SimpleMatrix normal = zRotationM.mult(xRotationM.mult(yRotationM.mult(normalvec)));
               SimpleMatrix LumM = normal.transpose().mult(L.transpose());

               double Lum = LumM.get(0,0);
               Lum = round(Lum*8);


               if(Lum>=0) {
                  Lum=Math.abs(Lum);
                  char value = getZVal(round(X), round(Y));
                  
                  if(value==0) {
                     setZBuf(round(X), round(Y), CHARACTERS.charAt((int)Lum));
                  } else if(CHARACTERS.indexOf(value)<=Lum){
                     setZBuf(round(X), round(Y), CHARACTERS.charAt((int)Lum));
                  }
                  
               }
            }
            // break;
            
         }

         clearScreen();
         // System.out.println(A);

         for(int i=0;i<zBuffer.length;++i) {
            System.out.print("               ");
            for(int j=0;j<zBuffer[0].length;++j) {
               if(CHARACTERS.indexOf(zBuffer[i][j])==-1){
                  System.out.print(" ");
               } else{
                  System.out.print(zBuffer[i][j]);
               }
            }
            System.out.print("\n");
         }
         
         A=(A+ASpacing)%(2*PI);
         B=(B+BSpacing)%(2*PI);
         
         clearBuffer();

         // Thread.sleep(10);
         // break;
      }

   }
}