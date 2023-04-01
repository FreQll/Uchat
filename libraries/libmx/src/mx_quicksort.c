#include "../inc/libmx.h"

int mx_quicksort(char **arr, int left, int right){
    if (!arr || !*arr) return -1;
    int count = 0;
    int i, j, pivot;
    char * temp;
    if(left < right){
        pivot=left;
        i=left;
        j=right;
        while(i < j){
            while(mx_strlen(arr[i]) <= mx_strlen(arr[pivot]) && i < right) i++;
            while(mx_strlen(arr[j]) > mx_strlen(arr[pivot])) j--;
            if(i < j){
                temp = arr[i];
                arr[i] = arr[j];
                arr[j]=temp;
                count += 1;
            }
        }
        if (mx_strlen(arr[j]) != mx_strlen(arr[pivot])){
            temp = arr[pivot];
            arr[pivot] = arr[j];
            arr[j] = temp;
            count += mx_quicksort(arr,left,j-1) + mx_quicksort(arr,j+1,right) + 1;
        }
        else 
            count += mx_quicksort(arr,left,j-1) + mx_quicksort(arr,j+1,right);
    }
   return count;
}



