#include "PrecRecCalculus.h"
#include "DistanceCalculus.h"
using namespace std;



/**
* Calls everything to generate the graphic
**/
PrecRecCalculus::PrecRecCalculus(Reader * r){
    reader = r;
    countPerCategory();
    /*//Verify category counters
    */
    for(int i = 0; i<NUMCAT;i++)
        printf("\ncat[%i]=%i",i,categories[i]);

    loadData();
    printf("\nDados Carregados com Sucesso");
    /*Verify data loaded in objects vector
    printf("\n*********************************\n");
    for(int i = 0; i< reader->getNbrObjs();i++){
        printf("\ni=%d; %.0f %.0f",i,objects[i][0],objects[i][1]);
        for(int j=2; j<reader->getNumberColumns(); j++)
            printf(" %.4f",objects[i][j]);
    } */

    calculate();
}

/**
* Finds categories and counts images occurences of it
**/
void PrecRecCalculus::countPerCategory(){
    int nbrobj=0;
    int cat;
    for(int i = 0; i< NUMCAT;i++){
        categories[i]=0;
    }
    reader->firstTransaction();
    do{
        cat = (int) reader->getValue(1);
        categories[cat]++;
        nbrobj++;
    }while(reader->nextLine()!=-1);
    reader->setNbrObjs(nbrobj);
}

/**
* Loads data in the object array
**/
void PrecRecCalculus::loadData(){
    reader->firstTransaction();
    float * t;
    int nbrcols = reader->getNumberColumns();
    int line = 0;
    do{
        t = reader->getFeatures();
        //object copy
        for(int i=0; i<nbrcols; i++){
            objects[line][i] =*t++;
        }
        line++;
    }while(reader->nextLine()!=-1);
    //scan all the database to them be the target
}

/**
* Calcules Precision and Recall for each image as target and for all
* k
**/

void PrecRecCalculus::calculate(){
    int n1=reader->getNbrObjs();
    for(int i=0;i<n1;i++){
        //meanprec[i]=0;
        //meanrec[i]=0;
    }
    /**
    * id, category, distance
    **/
    trifloat * rank = new trifloat[n1];
    int fim,k;
    int nbrcols= reader->getNumberColumns();
    DistanceCalculus * c1 = new DistanceCalculus(nbrcols);

    FILE * fp = fopen("outsave.txt","w");
    //fprintf(fp,"Precisão média por imagem");
    //selecting the target
    for( int i = 0; i < n1 ;i++){
        //scan all measures
        fim=-1;
        //printf("\nTarget:%.0f",objects[i][0]);
        for( int j = 0; j < n1 ;j++){
            float dist =c1->getDistance(objects[i],objects[j]);
            //printf("\nId=%.0f,cat=%0.f,Distance:%.4f",objects[j][0],objects[j][1],dist);
            k = fim;
            while(k>=0 && rank[k].dist > dist){
                rank[k+1].id = rank[k].id;
                rank[k+1].cat = rank[k].cat;
                rank[k+1].dist = rank[k].dist;
                k--;
            }
            rank[k+1].id = objects[j][0];
            rank[k+1].cat = objects[j][1];
            rank[k+1].dist = dist;
            fim++;
        }

        //Check rank values");
        /*for(int z=0;z<=fim; z++)
            printf("\nn=%d, id=%.0f, cat=%.0f, dist=%.4f",z,rank[z].id,rank[z].cat,rank[z].dist); */

        processranking(rank,(int)objects[i][1]);

//		char integer_string[32];
//		int integer = i;		
//		
//		sprintf(integer_string, "%d", integer);
//		
//		char str1[64] = "outsave"; 
//		strcat(str1, integer_string);
//		strcat(str1, ".txt");


        /**
        * Calcula do jeito do bayesa e yattes
        */


//			fprintf(fp,"\nset xlabel \"Recall (%%)\"");
//		    fprintf(fp,"\nset ylabel \"Precision (%%)\"");
//		    fprintf(fp,"\nset xrange [0:1]\nset yrange [0:1]");
//			fprintf(fp,"\nplot '-' title 'attribs=%d' with linespoints 1", reader->getNumberColumns()-2);
//			fprintf(fp,"\n####################");
//			fprintf(fp,"\n#attribs=%d",reader->getNumberColumns()-2);



        float p,pm,r,ct,oldp=0;
        int deltar = 5;
        pm=0;
        for(int rec=RECALLINTERVALS-1;rec>0;rec-=deltar){
            ct=0; p=0; r =0;
            for(k=0;k<deltar;k++){
                if(precision[rec-k][1]>0){
                    p += ((float)(precision[rec-k][0])/precision[rec-k][1]);
                    ct++;
                }
            }


            if(ct>0){
                r = (double)(rec)/100;
                p = p/(ct*100);



                if(oldp>p)
                    p=oldp;



                pm += p;


//	            fprintf(fp,"\n%.3f %.3f",r,p);
                printf("\nGrafico: %.3f %.3f",r,p);
                oldp=p;
            }


        }

        fprintf(fp,"\n%.3f", p);
//	       	fprintf(fp,"\n");

//	        fprintf(fp,"\n0 1");
//	        printf("\nGrafico:0 1");
//	        fprintf(fp,"\nend\npause -1");


    }

    fclose(fp);





}

/**
* Calculate precision and recall for all knn and adds the results to precision array
**/
void PrecRecCalculus::processranking(trifloat * rank, int cattarget){
    int prec=0,rec=0,correct=0,tmp=0;
    //printf("\ncat=%d\n",cattarget);
    for(int knn=0;knn<reader->getNbrObjs();knn++){
        tmp =(int)rank[knn].cat;
        if(tmp == cattarget){
            correct++;
            prec = (int)(correct*1000)/(knn+1);
            prec = (prec%10) >=5 ? prec/10 +1: prec/10;
            rec = (int)(correct*1000)/categories[cattarget];
            rec = (rec%10>=5)? rec/10 +1 : rec/10;
            precision[rec][0]+=prec; //adds precision value
            precision[rec][1]++; //adds precision counter
            //printf("\nknn=%d,prec=%d,rec=%d",knn,prec,rec);
        }
    }
}



