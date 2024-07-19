#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

// Misc

const int numbers_ascii_offset = (int) '0';
typedef struct{ int value; int valid;} MaybeInt;
MaybeInt quick_and_dirty_atoi(char* str, int start, int end){
  // Input is provided as a half open interval, [start, end)
  int new_value = 0;
  int atoi_valid = 1;

  int old_value = 0;
  for(int pos=start; pos < end; pos++){
    int a2i = ((int) str[pos]) - numbers_ascii_offset;
    if(a2i < 0 || a2i > 9) atoi_valid = 0; // non-alphanumerical character povided
    new_value = (new_value*10) + a2i;
    if(new_value < old_value) atoi_valid = 0; // overflow happened
    old_value = new_value;
  }

  MaybeInt answer;
  answer.valid = atoi_valid;
  if(atoi_valid) answer.value = new_value;
  
  return answer;
}


// Define struct

typedef enum{ XB, UV,} WordSize ; 
typedef enum{ PO, MR, SD,} Negative ;
typedef enum{ OE, OI, OR,} Overflow ; 
typedef enum{ UE, UI, UR,} Underflow ;
typedef struct{
    WordSize word_size; int word_size_value;
    Negative negative;
    Overflow overflow; 
    Underflow underflow;
} CV;

typedef enum{ M30K, XC, UC, } MaxCell ;
typedef enum{ AE, AI, AR, } AfterScope ; 
typedef enum{ BE, BI, BR, } BelowScope ;
typedef struct{
    MaxCell max_cell; int max_cell_value;
    AfterScope after_scope; 
    BelowScope below_scope;
} PT;


typedef enum{ DR, WI, } Read ; 
typedef enum{ FF, SW, } Write ;
typedef struct{ Read read; Write write; } IO;

typedef struct{
    CV cell_value;
    PT pointer;
    IO io;
    int version;
} BFClassification;

// Define default

void default_classification(BFClassification* classification){
  classification->cell_value.word_size = XB;
  classification->cell_value.word_size_value = 8;
  classification->cell_value.negative = PO;
  classification->cell_value.overflow = OE;
  classification->cell_value.underflow = UE;

  classification->pointer.max_cell = M30K;
  classification->pointer.max_cell_value = 0;
  classification->pointer.after_scope = AE;
  classification->pointer.below_scope = BE;

  classification->io.read = DR;
  classification->io.write = FF;

  classification->version = 0;
}

// Define parser

typedef enum{ STP_Unknown, STP_CV, STP_PT, STP_IO, STP_V, } ParseStep ;
typedef struct{ BFClassification classification; int valid;} MaybeBFClassification;
MaybeBFClassification parse(char* str, int start, int end){
  // Input is provided as a half open interval, [start, end)
  BFClassification parsed;
  default_classification(&parsed);
  int parsing_valid = 1;

  ParseStep step = STP_Unknown;
  ParseStep previous_step = STP_Unknown;
  int pos = start;
  while(pos < end &&  parsing_valid == 1){
    
    printf("pos: %i \n", pos);
    
    int pre_number_parse_pos = pos; // Parse number
    if(step == STP_V || step == STP_PT || step == STP_CV) 
    while(isdigit(str[pos]) && pos < end) pos++;

    int leftover = end - pos;

    switch(step){
      case STP_Unknown:

        if (str[pos] == 'c') { // CV
          if(leftover<=2 || str[pos+1] != 'v') parsing_valid = 0;
          if(leftover>=3 && str[pos+2] != '_' && str[pos+2] != '~') 
            parsing_valid = 0;
          step = STP_CV;
          pos = pos + 3;

        }else if (str[pos] == 'p'){ // PT
          if(leftover<=2 || str[pos+1] != 't') parsing_valid = 0;
          if(leftover>=3 && str[pos+2] != '_' && str[pos+2] != '~') 
            parsing_valid = 0;
          step = STP_PT;
          pos = pos + 3;

        }else if (str[pos] == 'i'){ // IO
          if(leftover<=2 || str[pos+1] != 'o') parsing_valid = 0;
          if(leftover>=3 && str[pos+2] != '_' && str[pos+2] != '~') 
            parsing_valid = 0;
          step = STP_IO;
          pos = pos + 3;

        }else if (str[pos] == 'v'){ // V
          if(leftover<2 || str[pos+1] != '_') {
            parsing_valid = 0; 
          } 
          pos= pos + 2;
          step = STP_V;
        }else{
          parsing_valid = 0;
        }

        if(previous_step > step) parsing_valid = 0; // Force step order
  
        break;

      case STP_CV:
          {

          if(pre_number_parse_pos == pos && leftover >= 2){ //
            // No numbers

            if (str[pos] == 'o') { 
              if(str[pos+1] == 'e'){ 
                parsed.cell_value.overflow = OE;
              }else if(str[pos+1] == 'i'){ 
                parsed.cell_value.overflow = OI;
              }else if(str[pos+1] == 'r'){ 
                parsed.cell_value.overflow = OR;
              }else{
                parsing_valid = 0;
              }
            } 
            
            else if (str[pos] == 'u') { 
              if(str[pos+1] == 'e'){ 
                parsed.cell_value.underflow = UE;
              }else if(str[pos+1] == 'i'){ 
                parsed.cell_value.underflow = UI;
              }else if(str[pos+1] == 'r'){ 
                parsed.cell_value.underflow = UR;
              }else{
                parsing_valid = 0;
              }
            }

            else if (str[pos] == 'u' && str[pos+1] == 'v') {
              parsed.cell_value.word_size = UV;
            } 

            else if (str[pos] == 'p' && str[pos+1] == 'o') {
              parsed.cell_value.negative = PO;
            } 

            else if (str[pos] == 'm' && str[pos+1] == 'r') {
              parsed.cell_value.negative = MR;
            } 

            else if (str[pos] == 's' && str[pos+1] == 'd') {
              parsed.cell_value.negative = SD;
            } 

            else{
              parsing_valid = 0;
            }

            if(leftover>=3){ 
              if(str[pos+2] == '~'){
                previous_step = STP_CV; 
                step = STP_Unknown;

              }
              else if(str[pos+2] != '_') {
                parsing_valid = 0;
              }
            }


            pos = pos + 3;

          }else if (pre_number_parse_pos != pos && pos < end){ //
            // Numbers
            
            if(str[pos]=='b'){
              parsed.cell_value.word_size = XB ;
              MaybeInt word_number;
              word_number = quick_and_dirty_atoi(str, pre_number_parse_pos, pos);
              parsing_valid = word_number.valid;
              if(word_number.valid) parsed.cell_value.word_size_value = word_number.value;
            }

            else{
              parsing_valid = 0;
            }

            pos++;
            
          }else{ //
            // Not enough chars
            parsing_valid = 0;
          }
        }
        break;

      case STP_PT:
        {

          if(pre_number_parse_pos == pos && leftover >= 2){ //
            // No numbers

            if (str[pos] == 'a') { 
              if(str[pos+1] == 'e'){ 
                parsed.pointer.after_scope = AE;
              }else if(str[pos+1] == 'i'){ 
                parsed.pointer.after_scope = AI;
              }else if(str[pos+1] == 'r'){ 
                parsed.pointer.after_scope = AR;
              }else{
                parsing_valid = 0;
              }
            } 
            
            else if (str[pos] == 'b') { 
              if(str[pos+1] == 'e'){ 
                parsed.pointer.below_scope = BE;
              }else if(str[pos+1] == 'i'){ 
                parsed.pointer.below_scope = BI;
              }else if(str[pos+1] == 'r'){ 
                parsed.pointer.below_scope = BR;
              }else{
                parsing_valid = 0;
              }
            }

            else if (str[pos] == 'u' && str[pos+1] == 'c') {
              parsed.pointer.max_cell = UC;
            } 

            else{
              parsing_valid = 0;
            }

            if(leftover>=3){ 
              if(str[pos+2] == '~'){
                previous_step = STP_PT; 
                step = STP_Unknown;

              }
              else if(str[pos+2] != '_') {
                parsing_valid = 0;
              }
            }


            pos = pos + 3;

          }else if (pre_number_parse_pos != pos && pos < end){ //
            // Numbers
            
            if( (pos - pre_number_parse_pos) == 2 && 
                  str[pos-2] == '3' && str[pos-1] == '0' && str[pos] == 'k'){
                    parsed.pointer.max_cell = M30K;
            }

            else if(str[pos]=='c'){
              parsed.pointer.max_cell = XC ;
              MaybeInt memory_number;
              memory_number = quick_and_dirty_atoi(str, pre_number_parse_pos, pos);
              parsing_valid = memory_number.valid;
              if(memory_number.valid) parsed.pointer.max_cell_value = memory_number.value;
            }

            else{
              parsing_valid = 0;
            }

            pos++;
            
          }else{ //
            // Not enough chars
            parsing_valid = 0;
          }
        }
        break;
      
      case STP_IO:

        if (leftover <= 1) parsing_valid = 0; 

        if(str[pos]=='d' && str[pos+1]=='r'){
          parsed.io.read = DR;
        }

        else if(str[pos]=='w' && str[pos+1]=='i'){
          parsed.io.read = WI;
        }

        else if(str[pos]=='f' && str[pos+1]=='f'){
          parsed.io.write = FF;
        }

        else if(str[pos]=='s' && str[pos+1]=='w'){
          parsed.io.write = SW;
        }
        
        else{
          parsing_valid = 0;
        }

        pos = pos + 2;

        if(pos < end){
          if(str[pos] == '~') {
            previous_step = STP_IO; 
            step = STP_Unknown;
          }

          else if(str[pos]!= '_'){
            parsing_valid = 0;
          }

          pos = pos + 1;
        }
        break;

      case STP_V:
        {

          if(pre_number_parse_pos == pos) parsing_valid = 0;
          
          if(parsing_valid){
            MaybeInt version_number = quick_and_dirty_atoi(str, pre_number_parse_pos, pos);
            parsing_valid = version_number.valid;
            if(version_number.valid) parsed.version = version_number.value;
            
            if(pos < end) {
              if (str[pos] != '~') parsing_valid = 0;
              pos++;
              previous_step = STP_V;
              step = STP_Unknown;
            }
          }
        }
        break;

      default: // Unknown Step
        parsing_valid = 0;
    }
  }

  
  MaybeBFClassification maybe_parsed;
  maybe_parsed.valid = parsing_valid;
  if(parsing_valid) maybe_parsed.classification = parsed;

  return maybe_parsed;
}

