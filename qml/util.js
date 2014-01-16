.pragma library

function timeToString(t)
{
    t = Math.round(t);
    var s = t % 60;
    var m = Math.floor(t / 60) % 60
    var h = Math.floor(t / 3600)

    return "%1:%2:%3".arg(h).arg(m < 10 ? ("0" + m) : m).arg(s < 10 ? ("0"+ s) : s);
}

function distanceToString(d)
{
    return qsTr("%1 km").arg((d / 1000).toLocaleString(Qt.locale() , "f", 2))
}

function speedToString(v)
{
    return qsTr("%1 km/h").arg((v * 3.6).toLocaleString(Qt.locale() , "f", 1))
}
