## roundtrip 

NTP协议 : 网络时间同步协议

用来同步客户端和服务端的时钟


                                                                              
```                                            
        T1                                                    T4       
.............................................................-... 
        . .               .                  .               . .             
        .  ..             .                  .             ..  .             
        .    .            .                  .            .    .             
            .                                        ..                   
        .       .         .                  .         . ..    .             
        .      .. .       .                  .       .   .     .             
        .       .  ..     .                  .     ..          .             
                    .                            .                          
        .              .  .                  .  ..             .             
        .               . .                  . .               .             
........................-..................:......................
                T2                       T3                       
                                                                            
```

RTT = (T4 - T1) - (T3 - T2)



T1 + RTT/2 = T2 + offset(客户端和服务端的时间差)