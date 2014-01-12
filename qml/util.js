.pragma library

function timeToString(t)
{
    t = Math.round(t);
    var s = t % 60;
    var m = Math.floor(t / 60) % 60
    var h = Math.floor(t / 3600)

    return "%1:%2:%3".arg(h).arg(m < 10 ? ("0" + m) : m).arg(s < 10 ? ("0"+ s) : s);
}
