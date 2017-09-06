var Service = require('node-windows').Service;

var svc = new Service({
    name: 'JLEDC1',
    description: 'Janho led controller',
    script: 'C:\\Users\\Miquel\\Documents\\Arduino\\JLEDC1\\nodejs_app\\bin\\www'
});

// Listen for the "install" event, which indicates the
// process is available as a service.
svc.on('install',function(){
    svc.start();
});
  
svc.install();