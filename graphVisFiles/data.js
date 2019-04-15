g = {
  nodes: [
    {id:'0',label:'0',x:0.300000,y:0.000000, size:0.2, color: getColor(1.000000,gradient)},
    {id:'1',label:'1',x:0.242705,y:0.176336, size:0.2, color: getColor(1.000000,gradient)},
    {id:'2',label:'2',x:0.092705,y:0.285317, size:0.2, color: getColor(1.000000,gradient)},
    {id:'3',label:'3',x:-0.092705,y:0.285317, size:0.2, color: getColor(1.000000,gradient)},
    {id:'4',label:'4',x:-0.242705,y:0.176336, size:0.2, color: getColor(1.000000,gradient)},
    {id:'5',label:'5',x:-0.300000,y:0.000000, size:0.2, color: getColor(1.000000,gradient)},
    {id:'6',label:'6',x:-0.242705,y:-0.176336, size:0.2, color: getColor(1.000000,gradient)},
    {id:'7',label:'7',x:-0.092705,y:-0.285317, size:0.2, color: getColor(1.000000,gradient)},
    {id:'8',label:'8',x:0.092705,y:-0.285317, size:0.2, color: getColor(1.000000,gradient)},
    {id:'9',label:'9',x:0.242705,y:-0.176336, size:0.2, color: getColor(1.000000,gradient)}
  ],
  edges: [
    {source:'0',target:'1',id:'0',label:'2',type: 'curvedArrow'},
    {source:'0',target:'2',id:'1',label:'5',type: 'curvedArrow'},
    {source:'0',target:'5',id:'2',label:'7',type: 'curvedArrow'},
    {source:'0',target:'7',id:'3',label:'100',type: 'curvedArrow'},
    {source:'0',target:'9',id:'4',label:'9',type: 'curvedArrow'},
    {source:'1',target:'4',id:'5',label:'1',type: 'curvedArrow'},
    {source:'1',target:'6',id:'6',label:'2',type: 'curvedArrow'},
    {source:'1',target:'3',id:'7',label:'4',type: 'curvedArrow'},
    {source:'2',target:'8',id:'8',label:'3',type: 'curvedArrow'},
    {source:'3',target:'8',id:'9',label:'4',type: 'curvedArrow'},
    {source:'5',target:'1',id:'10',label:'6',type: 'curvedArrow'},
    {source:'7',target:'5',id:'11',label:'2',type: 'curvedArrow'},
    {source:'7',target:'2',id:'12',label:'6',type: 'curvedArrow'},
    {source:'9',target:'4',id:'13',label:'2',type: 'curvedArrow'}
  ]
}