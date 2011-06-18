# R U OK

A program to measures timings of transferring data with URL syntax and helps identify where in the pipeline the problem exists, if at all.

## Libraries needed 
* curl >= 7.21.0
* libxml2 >= 2.7.8
* cmake >= 2.6

## Compilation 

1. cmake .
2. make 
3. make install

## Parameters
<pre>
   Allowed options:
     -f [ --follow ]                       Follow redirects
     -h [ --help ]                         Displays this information
     -H [ --Header ] arg ( "Host:www.example.com" )     Stackable parameter to pass HTTP headers to the request
     -j [ --json ]                         Verify JSON validity
     -m [ --miliseconds ]                  Display timings in miliseconds ( default seconds )
     -n [ --no-header ]                    Do not display the header
     -p [ --period ] arg (=10)             Period in seconds
     -r [ --rate ] arg (=1)                Rate of requests per second
     -u [ --url ] arg                      Url to test
     -U [ --user-agent ] arg (=Mozilla/5.0 (Macintosh; U; Intel Mac OS X 10_5_8; en-us) AppleWebKit/533.21.1 (KHTML, like Gecko) Version/5.0.5 Safari/533.21.1)
                                           User agent to use
     -v [ --verbose ]                      Be verbose
     -V [ --version ]                      Print version string
     -x [ --xml ]                          Verify XML validity
</pre>

## Example usage 

* By default rate is 1 request per second for 10 seconds

ruok -u http://www.google.com/search?q=ruok
<pre>
DNS                 Connect             SSL                 Protocol            First byte          Total               Bytes               Return code         
0.000934            0.109248            0.000000            0.000001            0.153485            1.320860            96679.000000        200                 
0.001100            0.107790            0.000000            0.000001            0.146549            1.639408            96680.000000        200                 
0.000691            0.117595            0.000000            0.000000            0.147891            1.113251            81004.000000        200                 
0.000499            0.117757            0.000000            0.000001            0.135330            1.282660            96599.000000        200                 
</pre>

* Turn on XML validation

ruok -u http://www.google.com/search?q=ruok -x
<pre>
DNS                 Connect             SSL                 Protocol            First byte          Total               Bytes               Return code         XML             
0.000799            0.107808            0.000000            0.000000            0.141417            1.258222            96699.000000        200                 NOT OK     
0.000855            0.108051            0.000000            0.000005            0.151036            1.573901            96721.000000        200                 NOT OK     
0.000323            0.115909            0.000000            0.000001            0.153521            1.506323            96580.000000        200                 NOT OK     
0.000899            0.115320            0.000000            0.000002            0.142294            1.507822            96363.000000        200                 NOT OK     
</pre>

## Thanks

* Peter Ellehauge for a very cool name suggestion and testing
