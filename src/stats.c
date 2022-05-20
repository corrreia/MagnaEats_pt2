#include "../include/stats.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

FILE * out_file;
extern int op_counter;

void createStatsFile(char * fileName){ //create stats file
    out_file = fopen (fileName, "w"); 
}

void doStats(struct main_data* data){
    fputs("Process Statistics:\n",out_file);
    for(int i = 0; i< data->n_restaurants;i++){ //for each restaurant print stats
        fputs("        Restaurante ",out_file);
        fprintf(out_file,"%d",i);    
        fputs(" preparou ",out_file);
        fprintf(out_file,"%d",data->restaurant_stats[i]);         
        fputs(" pedidos!\n",out_file);      
    }
    fputs("\n",out_file);
    for(int i = 0; i< data->n_drivers;i++){ //for each driver print stats
        fputs("        Motorista ",out_file);
        fprintf(out_file,"%d",i);    
        fputs(" entregou ",out_file);
        fprintf(out_file,"%d",data->driver_stats[i]);         
        fputs(" pedidos!\n",out_file);  
    }
    fputs("\n",out_file);
    for(int i = 0; i< data->n_clients;i++){ //for each client print stats
        fputs("        Cliente ",out_file);
        fprintf(out_file,"%d",i);    
        fputs(" recebeu ",out_file);
        fprintf(out_file,"%d",data->client_stats[i]);         
        fputs(" pedidos!\n",out_file);  
    }
    fputs("\nRequest Statistics:\n\n",out_file);
    
    
    for(int i = 0; i<op_counter; i++){ //for each request print stats
        fputs("        Request: ",out_file);
        fprintf(out_file,"%d",data->results[i].id);   
        fputs("\n",out_file);
        
        fputs("        Status: ",out_file);
        fprintf(out_file,"%c",data->results[i].status);   
        fputs("\n",out_file);

        
        if(data->results[i].status == 'C'){
            
            fputs("        Restaurant id: ",out_file);
            fprintf(out_file,"%d",data->results[i].receiving_rest);   
            fputs("\n",out_file);
            
            fputs("        Driver id: ",out_file);
            fprintf(out_file,"%d",data->results[i].receiving_driver);   
            fputs("\n",out_file);

            fputs("        Client id: ",out_file);
            fprintf(out_file,"%d\n",data->results[i].receiving_client);   

            char t [40];

            fputs("        Created: ",out_file);
            strftime(t,40,"%F %X",localtime( &data->results[i].start_time.tv_sec));
            fprintf(out_file,"%s.%ld\n",t, data->results[i].start_time.tv_nsec/1000000);

            fputs("        Restaurant time: ",out_file);
            strftime(t,40,"%F %X",localtime( &data->results[i].rest_time.tv_sec));
            fprintf(out_file,"%s.%ld\n",t, data->results[i].rest_time.tv_nsec/1000000);  
            
            fputs("        Driver time: ",out_file);
            strftime(t,40,"%F %X",localtime( &data->results[i].driver_time.tv_sec));
            fprintf(out_file,"%s.%ld\n",t, data->results[i].driver_time.tv_nsec/1000000);
            

            fputs("        Client time (end): ",out_file);
            strftime(t,40,"%F %X",localtime( &data->results[i].client_end_time.tv_sec));
            fprintf(out_file,"%s.%ld\n",t, data->results[i].client_end_time.tv_nsec/1000000);
            
            fputs("        Total Time: ",out_file);
            fprintf(out_file,"%ld.%ld s (%ld ns)\n\n",
            (data->results[i].client_end_time.tv_sec - data->results[i].start_time.tv_sec),
            (data->results[i].client_end_time.tv_nsec - data->results[i].start_time.tv_nsec)/1000000,
            (data->results[i].client_end_time.tv_nsec - data->results[i].start_time.tv_nsec));
        }
    }
}

void closeStatsFile(){  //stats close file
    fclose(out_file);
}