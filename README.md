# SocketProgramming

You are to write two programs:
1. A client program that will connect with your
server and send it requests for the weather report
for a given city.

2. A server program that will wait for connection
requests from your client and exchange one-line
text messages with it.

The Server Program
Your server must start by reading in a file named
weather20.txt that will contain a list of cities with their
next day maximum temperatures and sky conditions as in:
Amarillo,61,Sunny
Austin,76,Partly Cloudy
Corpus Christi,79,AM Thunderstorms
Dallas,65,Sunny
El Paso,67,Sunny
Galveston,69,Thunderstorms
Houston,77,AM Thunderstorms
San Antonio,79,Mostly Cloudy
with the three field separated by commas. It should then prompt for a port to listen to as in
Enter server port number: 2468

It will then create a stream socket, bind it to the
specified port number, do a listen() to specify a
maximum number of queued connection requests and do
an accept() that will let it wait for connection requests.
Whenever the server accepts a connection request, it
will receive a city name and reply to the client with the
day’s maximum temperature and sky condition.

Additionally—and for debugging purposes—the server
should print out the name of the city in the client request,
its next day maximum temperature and its sky condition
as in:
Weather report for Houston
Tomorrow’s maximum temperature is 77 F
Tomorrow’s sky condition is AM Thunderstorms
or
Weather report for Brussels
No data

Once it has done it, it should send to the client a single
message with the requested data.

The Client Program
Your client should start by prompting the user for a
server host name and a server port number as in:
Enter the server host name: localhost
Enter the server port number: 2468

Please note that localhost is the sole correct answer
if you run your two programs on a computer lacking a full
internet name.

It should then create a stream socket, do a
connect() request to the specified server, and prompt
the user for a city name, and send that name to the server:
Enter a city name: Corpus Christi
Tomorrow’s maximum temperature is 78 F
Tomorrow’s sky condition is AM Thunderstorms
or
Enter a city name: Brussels
No data

