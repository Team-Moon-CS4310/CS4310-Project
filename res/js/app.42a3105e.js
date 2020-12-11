(function(e){function t(t){for(var l,r,a=t[0],u=t[1],s=t[2],c=0,d=[];c<a.length;c++)r=a[c],Object.prototype.hasOwnProperty.call(i,r)&&i[r]&&d.push(i[r][0]),i[r]=0;for(l in u)Object.prototype.hasOwnProperty.call(u,l)&&(e[l]=u[l]);f&&f(t);while(d.length)d.shift()();return o.push.apply(o,s||[]),n()}function n(){for(var e,t=0;t<o.length;t++){for(var n=o[t],l=!0,a=1;a<n.length;a++){var u=n[a];0!==i[u]&&(l=!1)}l&&(o.splice(t--,1),e=r(r.s=n[0]))}return e}var l={},i={app:0},o=[];function r(t){if(l[t])return l[t].exports;var n=l[t]={i:t,l:!1,exports:{}};return e[t].call(n.exports,n,n.exports,r),n.l=!0,n.exports}r.m=e,r.c=l,r.d=function(e,t,n){r.o(e,t)||Object.defineProperty(e,t,{enumerable:!0,get:n})},r.r=function(e){"undefined"!==typeof Symbol&&Symbol.toStringTag&&Object.defineProperty(e,Symbol.toStringTag,{value:"Module"}),Object.defineProperty(e,"__esModule",{value:!0})},r.t=function(e,t){if(1&t&&(e=r(e)),8&t)return e;if(4&t&&"object"===typeof e&&e&&e.__esModule)return e;var n=Object.create(null);if(r.r(n),Object.defineProperty(n,"default",{enumerable:!0,value:e}),2&t&&"string"!=typeof e)for(var l in e)r.d(n,l,function(t){return e[t]}.bind(null,l));return n},r.n=function(e){var t=e&&e.__esModule?function(){return e["default"]}:function(){return e};return r.d(t,"a",t),t},r.o=function(e,t){return Object.prototype.hasOwnProperty.call(e,t)},r.p="/";var a=window["webpackJsonp"]=window["webpackJsonp"]||[],u=a.push.bind(a);a.push=t,a=a.slice();for(var s=0;s<a.length;s++)t(a[s]);var f=u;o.push([0,"chunk-vendors"]),n()})({0:function(e,t,n){e.exports=n("56d7")},"034f":function(e,t,n){"use strict";n("85ec")},"56d7":function(e,t,n){"use strict";n.r(t);n("e260"),n("e6cf"),n("cca6"),n("a79d");var l,i,o=n("2b0e"),r=function(){var e=this,t=e.$createElement,n=e._self._c||t;return n("div",{attrs:{id:"app"}},[n("FileInput"),n("FileDisplay")],1)},a=[],u=function(){var e=this,t=e.$createElement,n=e._self._c||t;return n("div",[n("h1",[e._v("C++/Vue.js Cloud Storage")]),n("br"),n("input",{ref:"fileUploading",attrs:{type:"file"},on:{change:function(t){return e.selectedFileChange()}}}),n("b-button",{staticClass:"m-3 btn-info",on:{click:function(t){return e.uploadFile()}}},[e.uploadLoading?n("span",{staticClass:"spinner-border spinner-border-sm"}):e._e(),e._v(" Upload File ")]),e.filesLoading?n("div",[n("br"),n("div",{staticClass:"spinner-border"})]):e._e(),e.deleteLoading?n("span",{staticClass:"spinner-border"}):e._e(),null!=e.files?n("b-list-group",e._l(e.files,(function(t,l){return n("b-list-group-item",{key:l},[n("b-button",{staticClass:"btn-info",attrs:{href:"/file/"+t}},[e._v(e._s(t))]),n("b-button",{staticClass:"m-3 btn-danger",on:{click:function(n){return e.deleteFile(t)}}},[e._v("Delete")])],1)})),1):e._e()],1)},s=[],f=n("d4ec"),c=n("bee2"),d=n("bc3a"),p=n.n(d),b=function(){function e(){Object(f["a"])(this,e)}return Object(c["a"])(e,[{key:"getFiles",value:function(){return p.a.get("/getAllFiles")}},{key:"deleteFile",value:function(e){return p.a.delete(e)}},{key:"uploadFile",value:function(e){var t=new FormData;return t.append("file",e),p.a.post("/upload",t)}}]),e}(),g=new b,h={name:"FileDisplay",data:function(){return{files:null,fileToUpload:null,filesLoading:!0,uploadLoading:!1,deleteLoading:!1}},methods:{getFiles:function(){var e=this;g.getFiles().then((function(t){e.files=t.data,e.filesLoading=!1})).catch((function(e){console.log(e)}))},deleteFile:function(e){var t=this;this.deleteLoading=!0,g.deleteFile(e).then((function(e){200===e.status&&(t.files=e.data,t.deleteLoading=!1)}))},selectedFileChange:function(e){this.fileToUpload=this.$refs.fileUploading.files[0]},uploadFile:function(){var e=this;this.uploadLoading=!0,g.uploadFile(this.fileToUpload).then((function(t){200===t.status&&(e.files=t.data,e.uploadLoading=!1,e.$refs.fileUploading.value=null)}))}},mounted:function(){this.getFiles()}},v=h,y=(n("e120"),n("2877")),F=Object(y["a"])(v,u,s,!1,null,null,null),m=F.exports,_={name:"FileInput"},O=_,j=Object(y["a"])(O,l,i,!1,null,null,null),w=j.exports,L={name:"App",components:{FileDisplay:m,FileInput:w}},C=L,k=(n("034f"),Object(y["a"])(C,r,a,!1,null,null,null)),x=k.exports,P=n("5f5b"),U=n("b1e0");n("f9e3"),n("2dd8");o["default"].use(P["a"]),o["default"].use(U["a"]),o["default"].config.productionTip=!1,new o["default"]({render:function(e){return e(x)}}).$mount("#app")},"767f":function(e,t,n){},"85ec":function(e,t,n){},e120:function(e,t,n){"use strict";n("767f")}});
//# sourceMappingURL=app.42a3105e.js.map