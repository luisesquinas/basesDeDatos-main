document.getElementById('envioSQL').onsubmit = async function (e) {
    e.preventDefault();

    sql = document.getElementById('sql').value;
    
    response = await fetch(`/cgi-bin/funciones.cgi`,{
        method: 'POST',
        headers: {
            'Content-Type': 'application/x-www-form-urlencoded'
        },
        body: new URLSearchParams({
            'q':sql
        })
    });
    data = await response.text();
    
    document.getElementById('resultado').innerHTML = sql + decodeURI(data);
    document.getElementByTagName('textarea')[0].value = "";
    
};


document.getElementsByTagName('body')[0].onload = async function (e) {
    e.preventDefault();

    sql = "SELECT * FROM Personas";
    
    response = await fetch(`/cgi-bin/funciones.cgi`,{
        method: 'POST',
        headers: {
            'Content-Type': 'application/x-www-form-urlencoded'
        },
        body: new URLSearchParams({
            'q':sql
        })
    });
    data = await response.text();
    
    document.getElementById('resultado').innerHTML = decodeURI(data);
    
};
