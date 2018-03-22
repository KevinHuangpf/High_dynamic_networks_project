
# GPSR routing agent settings
for {set i 0} {$i < $opt(nn)} {incr i} {
    $ns_ at 0.00002 "$ragent_($i) turnon"
    $ns_ at 20.0 "$ragent_($i) neighborlist"
#    $ns_ at 30.0 "$ragent_($i) turnoff"
}

$ns_ at 10.5 "$ragent_(19) startSink 10.0"
$ns_ at 11.0 "$ragent_(70) startSink 10.0"
$ns_ at 11.5 "$ragent_(40) startSink 10.0"
$ns_ at 12.0 "$ragent_(71) startSink 10.0"
$ns_ at 12.5 "$ragent_(87) startSink 10.0"
$ns_ at 13.0 "$ragent_(23) startSink 10.0"
$ns_ at 13.5 "$ragent_(96) startSink 10.0"
$ns_ at 14.0 "$ragent_(88) startSink 10.0"
$ns_ at 14.5 "$ragent_(75) startSink 10.0"
$ns_ at 15.0 "$ragent_(9) startSink 10.0"


# GPSR routing agent dumps
$ns_ at 25.0 "$ragent_(24) sinklist"


# Upper layer agents/applications behavior
set null_(1) [new Agent/Null]
$ns_ attach-agent $node_(19) $null_(1)

set udp_(1) [new Agent/UDP]
$ns_ attach-agent $node_(33) $udp_(1)

set cbr_(1) [new Application/Traffic/CBR]
$cbr_(1) set packetSize_ 512
$cbr_(1) set interval_ 0.5
$cbr_(1) set random_ 1
#    $cbr_(1) set maxpkts_ 100
$cbr_(1) attach-agent $udp_(1)
$ns_ connect $udp_(1) $null_(1)
$ns_ at 76.0 "$cbr_(1) start"
$ns_ at 150.0 "$cbr_(1) stop" 


set udp_(2) [new Agent/UDP]
$ns_ attach-agent $node_(27) $udp_(2)

set null_(2) [new Agent/Null]
$ns_ attach-agent $node_(70) $null_(2)

set cbr_(2) [new Application/Traffic/CBR]
$cbr_(2) set packetSize_ 512
$cbr_(2) set interval_ 0.5
$cbr_(2) set random_ 1
#    $cbr_(2) set maxpkts_ 2
$cbr_(2) attach-agent $udp_(2)
$ns_ connect $udp_(2) $null_(2)
$ns_ at 76.0 "$cbr_(2) start"
$ns_ at 150.0 "$cbr_(2) stop" 



set udp_(3) [new Agent/UDP]
$ns_ attach-agent $node_(90) $udp_(3)

set null_(3) [new Agent/Null]
$ns_ attach-agent $node_(40) $null_(3)

set cbr_(3) [new Application/Traffic/CBR]
$cbr_(3) set packetSize_ 512
$cbr_(3) set interval_ 0.5
$cbr_(3) set random_ 1
#    $cbr_(3) set maxpkts_ 2
$cbr_(3) attach-agent $udp_(3)
$ns_ connect $udp_(3) $null_(3)
$ns_ at 76.0 "$cbr_(3) start"
$ns_ at 150.0 "$cbr_(3) stop" 

set udp_(4) [new Agent/UDP]
$ns_ attach-agent $node_(49) $udp_(4)

set null_(4) [new Agent/Null]
$ns_ attach-agent $node_(71) $null_(4)

set cbr_(4) [new Application/Traffic/CBR]
$cbr_(4) set packetSize_ 512
$cbr_(4) set interval_ 0.5
$cbr_(4) set random_ 1
#    $cbr_(4) set maxpkts_ 2
$cbr_(4) attach-agent $udp_(4)
$ns_ connect $udp_(4) $null_(4)
$ns_ at 76.0 "$cbr_(4) start"
$ns_ at 150.0 "$cbr_(4) stop" 

set udp_(5) [new Agent/UDP]
$ns_ attach-agent $node_(44) $udp_(5)

set null_(5) [new Agent/Null]
$ns_ attach-agent $node_(87) $null_(5)

set cbr_(5) [new Application/Traffic/CBR]
$cbr_(5) set packetSize_ 512
$cbr_(5) set interval_ 0.5
$cbr_(5) set random_ 1
#    $cbr_(5) set maxpkts_ 2
$cbr_(5) attach-agent $udp_(5)
$ns_ connect $udp_(5) $null_(5)
$ns_ at 76.0 "$cbr_(5) start"
$ns_ at 150.0 "$cbr_(5) stop" 

set udp_(6) [new Agent/UDP]
$ns_ attach-agent $node_(60) $udp_(6)

set null_(6) [new Agent/Null]
$ns_ attach-agent $node_(23) $null_(6)

set cbr_(6) [new Application/Traffic/CBR]
$cbr_(6) set packetSize_ 512
$cbr_(6) set interval_ 0.5
$cbr_(6) set random_ 1
#    $cbr_(6) set maxpkts_ 2
$cbr_(6) attach-agent $udp_(6)
$ns_ connect $udp_(6) $null_(6)
$ns_ at 76.0 "$cbr_(6) start"
$ns_ at 150.0 "$cbr_(6) stop" 

set udp_(7) [new Agent/UDP]
$ns_ attach-agent $node_(43) $udp_(7)

set null_(7) [new Agent/Null]
$ns_ attach-agent $node_(96) $null_(7)

set cbr_(7) [new Application/Traffic/CBR]
$cbr_(7) set packetSize_ 512
$cbr_(7) set interval_ 0.5
$cbr_(7) set random_ 1
#    $cbr_(7) set maxpkts_ 2
$cbr_(7) attach-agent $udp_(7)
$ns_ connect $udp_(7) $null_(7)
$ns_ at 76.0 "$cbr_(7) start"
$ns_ at 150.0 "$cbr_(7) stop" 

set udp_(8) [new Agent/UDP]
$ns_ attach-agent $node_(53) $udp_(8)

set null_(8) [new Agent/Null]
$ns_ attach-agent $node_(88) $null_(8)

set cbr_(8) [new Application/Traffic/CBR]
$cbr_(8) set packetSize_ 512
$cbr_(8) set interval_ 0.5
$cbr_(8) set random_ 1
#    $cbr_(8) set maxpkts_ 2
$cbr_(8) attach-agent $udp_(8)
$ns_ connect $udp_(8) $null_(8)
$ns_ at 76.0 "$cbr_(8) start"
$ns_ at 150.0 "$cbr_(8) stop" 

set udp_(9) [new Agent/UDP]
$ns_ attach-agent $node_(5) $udp_(9)

set null_(9) [new Agent/Null]
$ns_ attach-agent $node_(75) $null_(9)

set cbr_(9) [new Application/Traffic/CBR]
$cbr_(9) set packetSize_ 512
$cbr_(9) set interval_ 0.5
$cbr_(9) set random_ 1
#    $cbr_(9) set maxpkts_ 2
$cbr_(9) attach-agent $udp_(9)
$ns_ connect $udp_(9) $null_(9)
$ns_ at 76.0 "$cbr_(9) start"
$ns_ at 150.0 "$cbr_(9) stop" 

set udp_(10) [new Agent/UDP]
$ns_ attach-agent $node_(4) $udp_(10)

set null_(10) [new Agent/Null]
$ns_ attach-agent $node_(9) $null_(10)

set cbr_(10) [new Application/Traffic/CBR]
$cbr_(10) set packetSize_ 512
$cbr_(10) set interval_ 0.5
$cbr_(10) set random_ 1
#    $cbr_(10) set maxpkts_ 2
$cbr_(10) attach-agent $udp_(10)
$ns_ connect $udp_(10) $null_(10)
$ns_ at 76.0 "$cbr_(10) start"
$ns_ at 150.0 "$cbr_(10) stop" 
