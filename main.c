#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdint.h>

#define DEFAULT_BUFF_SIZE 20
#define MAX_IP4_ADDR_LEN 15
#define MAX_DIGITS_PER_BYTE 3

enum {
    EXIT_DISCONN,
    EXIT_W_ERROR,
    EXIT_R_ERROR,
    EXIT_GETHOSTNAME_ERROR,
    EXIT_SOCKET_ERROR,
    EXIT_CONNECT_ERROR,
    EXIT_BAD_PORT_ERROR,
    EXIT_MEM_ALLOC_ERROR
};

/**
 * Reads from stdin.
 *
 * @param buffer A buffer to place the bytes in
 * @return total bytes(integers) read
 */
size_t get_bytes_in_line( uint8_t buffer[], const size_t buffer_size ) {
    puts( "Enter byte values separated by a space up to 20(default) or the value that you provided as a cli arg" );
    size_t bytes_scanned = 0;
    size_t max_line_size = buffer_size * MAX_DIGITS_PER_BYTE // space for digits, assuming user is a good cst student...
                           + buffer_size; // space for whitespace in-between + null byte
    char * line_buffer = malloc( sizeof( char ) * max_line_size );
    if (line_buffer == NULL) {
        perror("Error allocating memory at get bytes in line");
        exit(EXIT_MEM_ALLOC_ERROR);
    }
    line_buffer[ max_line_size ] = 0;
    getline( &line_buffer, &max_line_size, stdin );

    char * end_ptr;
    size_t byte_read = strtol( line_buffer, &end_ptr, 10 );
    while ( *end_ptr != '\n' ) {
        if ( 255 < byte_read ) {
            buffer[ bytes_scanned ] = 255;
        } else {
            buffer[ bytes_scanned ] = byte_read;
        }
        bytes_scanned++;
        byte_read = strtol( end_ptr, &end_ptr, 10 );
    }
    if ( 255 < byte_read ) {
        buffer[ bytes_scanned ] = 255;
    } else {
        buffer[ bytes_scanned ] = byte_read;
    }
    bytes_scanned++;
    free(line_buffer);
    return bytes_scanned;
}

/**
 * Sends bytes in buffer to server.
 */
size_t send_bytes( const int server_fd, const uint8_t buffer[], const size_t bytes_to_write ) {
    int bytes_sent = write( server_fd, buffer, bytes_to_write );
    if ( bytes_sent == -1 ) {
        perror( "Error sending bytes" );
        exit( EXIT_W_ERROR );
    }
    if ( bytes_sent == 0 ) {
        perror( "It seems the server has disconnected" );
        exit( EXIT_DISCONN );
    }
    return bytes_sent;
}

/**
 * Receive bytes into buffer.
 */
size_t recv_bytes( const int server_fd, uint8_t buffer[], const size_t buffer_size ) {
    int bytes_read = read( server_fd, buffer, buffer_size );
    if ( bytes_read == -1 ) {
        perror( "Error receiving bytes." );
        exit( EXIT_R_ERROR );
    }
    if ( bytes_read == 0 ) {
        perror( "It seems the server has disconnected" );
        exit( EXIT_DISCONN );
    }
    return bytes_read;
}


/**
 * Print the bytes in buffer in decimal base.
 */
void print_bytes( const uint8_t buffer[], const size_t bytes_in_buff ) {
    for ( int i = 0; i < bytes_in_buff; ++i ) {
        printf( "%d ", buffer[ i ] );
    }
    puts( "" );
}

/**
 * Receive bytes into buffer.
 * @return
 */
int get_server_fd( char * ip4_addr, int port ) {
    struct hostent * host_info;
    struct sockaddr_in addr;
    int fd;
    host_info = gethostbyname( ip4_addr );
    if ( host_info == NULL) {
        perror( "Error at get host by name" );
        exit( EXIT_GETHOSTNAME_ERROR );
    }
    fd = socket( AF_INET, SOCK_STREAM, 0 );
    if ( fd == -1 ) {
        perror( "Error at creating socket" );
        exit( EXIT_SOCKET_ERROR );
    }
    memset( &addr, 0, sizeof( struct sockaddr_in ));
    addr.sin_family = AF_INET;
    addr.sin_port = htons( port );
    addr.sin_addr = *( struct in_addr * ) host_info->h_addr;
    int conn_status = connect( fd, ( struct sockaddr * ) &addr, sizeof( struct sockaddr_in ));
    if ( conn_status == -1 ) {
        perror( "Error at creating socket" );
        exit( EXIT_CONNECT_ERROR );
    }
    return fd;
}


void flush_stdin() {
    while (getc(stdin) != '\n' ){
        puts("fk");
    };
}


/**
 * Query user for ip address and scan it.
 * @param buff Must be sized appropriately to contain a 12 digit ip4 address and a null byte
 */
void get_ip( char * buff ) {
    puts( "Please enter the IP4 address of the server as such: #.#.#.#" );
    scanf( "%s", buff );
    flush_stdin();
}

/**
 * Query user for port number and scan it.
 * @return the port
 */
int get_port() {
    puts( "Please enter the port to connect to as such: #\nNumber entered must be as such: 0 < port < 65536" );

    int buff;
    scanf( "%d", &buff );
    if ( buff < 1 || 65535 < buff ) {
        perror( "You need to enter a port number such as: 0 < port < 65536" );
        exit( EXIT_BAD_PORT_ERROR );
    }
    flush_stdin();
    return buff;
}

int main( int argc, char * argv[] ) {
    size_t buff_size;
    if ( argc == 2 ) {
        buff_size = atoi( argv[ 1 ] );
    } else {
        buff_size = DEFAULT_BUFF_SIZE;
    }
    char server_ip4[MAX_IP4_ADDR_LEN + 1];
    server_ip4[ MAX_IP4_ADDR_LEN ] = 0;
    get_ip( server_ip4 );

    int port = get_port();


    int server_fd = get_server_fd( server_ip4, port );

    uint8_t buffer[buff_size + 1];
    size_t buff_bytes_count;
    while ( 1 ) {
        buff_bytes_count = get_bytes_in_line( buffer, buff_size );
        send_bytes( server_fd, buffer, buff_bytes_count );
        buff_bytes_count = recv_bytes( server_fd, buffer, buff_size );
        print_bytes( buffer, buff_bytes_count );
    }
}
