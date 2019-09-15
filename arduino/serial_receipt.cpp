
// CIRCULAR BUFFER DEFINITION

#define CF_INC(x, N) x = ((x)+1)%N

struct CircularBuffer{
  int n, ini, fin, len;
  char *ar;
};
typedef struct CircularBuffer Circularbuffer;

void cf_init(CircularBuffer *cf, char *ar, int len);
void cf_push(CircularBuffer *cf, char c);
char cf_pop(CircularBuffer *cf);
int cf_size(CircularBuffer *cf);



// RAISEBUTTON DEFINITION

struct RaiseButton{
  char b0, b1, pin;
};
typedef struct RaiseButton RaiseButton;

void rs_init(RaiseButton *rb, char pin);
char rs_pressed(RaiseButton *rb);





// CIRCULAR BUFFER IMPLEMENTATION

void cf_init(CircularBuffer *cf, char *ar, int len){
  cf->n = 0;
  cf->ini = 0;
  cf->fin = 0;
  cf->ar = ar;
  cf->len = len;
}

void cf_push(CircularBuffer *cf, char c){
  cf->ar[cf->fin] = c;
  CF_INC(cf->fin, cf->len);
  if ( cf->n<cf->len ){
    cf->n++;
  } else {
    CF_INC(cf->ini, cf->len);
  }
}

char cf_pop(CircularBuffer *cf){
  char c = (char)0;
  if(cf->n){
    c = cf->ar[cf->ini];
    CF_INC(cf->ini, cf->len);
    cf->n--;
  }
  return c;
}

int cf_size(CircularBuffer *cf){
  return cf->n;
}



// RAISE BUTTON IMPLEMENTATION


void rs_init(RaiseButton *rb, char pin){
  rb->b0 = 0;
  rb->b1 = 0;
  rb->pin = pin;
  pinMode(pin, INPUT);
}

char rs_pressed(RaiseButton *rb){
  rb->b1 = digitalRead(rb->pin);
  char ans = rb->b0==0 && rb->b1==1;
  rb->b0 = rb->b1;
  return ans;
}




// MAIN PROGRAM

HardwareSerial *computer, *printer;
#define BUF_LEN 1024
char buf[BUF_LEN];
CircularBuffer cf;
#define FLUSH_PIN 8
RaiseButton fbutton;
char tbuf[64];
char tbuf_read;



void setup() {
	// flush BUTTON
	rs_init(&fbutton, FLUSH_PIN);
	// computer 
	computer = &Serial2;
	computer->begin(9600);
	// printer 
	printer = &Serial3;
	printer->begin(9600);
	printer->write(0x1B);
	printer->write('@');
	// buffer 
	cf_init(&cf, buf, BUF_LEN);
	delay(100);
}

void loop() {
	while( computer->available() ){
		tbuf_read = computer->readBytes(tbuf, 64);
		for(int i=0;i<tbuf_read;i++){
			cf_push(&cf, tbuf[i]);
		}
	}

	if( rs_pressed(&fbutton) ){
		while( cf_size(&cf) ){
			char c = cf_pop(&cf);
			printer->write(c);
		}
	}
}