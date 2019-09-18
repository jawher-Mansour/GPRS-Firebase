const functions = require('firebase-functions');
const admin = require('firebase-admin');
admin.initializeApp();

// the multipe write function
exports.addMessage = functions.https.onRequest((req, res) => {
const demand = req.query
for (var i in demand){
subdemand = JSON.parse(demand[i]);
admin.database().ref('/').child(i).update(subdemand);
}
return res.sendStatus(200);
});

// the multiple read function
 exports.helloWorld = functions.https.onRequest((req, res) => {
const demand = req.query;
return admin.database().ref('/').once('value').then((snapshot)=>{
var string = "";
for (var i in demand){
subdemand = JSON.parse(demand[i]);
for (var j in subdemand){
string +=" "+ snapshot.child(i).child(subdemand[j]).val();
}
}
    return res.send(string);
});
 });

// single write function
exports.write = functions.https.onRequest((req, res) => {
const demand = req.query;
const name = demand.device;
delete demand.device;
  admin.database().ref('/').child(name).update(demand);
return res.send('operation successful');
});

// single read function
exports.read = functions.https.onRequest((req,res)=>{
const demand = req.query;
const name = demand.device;
delete demand.device;
var string = "";
return admin.database().ref('/').once('value').then((snapshot)=>{
for (var i in demand){
string +=" "+ snapshot.child(name).child(demand[i]).val();
}
    return res.send(string);
});
});
