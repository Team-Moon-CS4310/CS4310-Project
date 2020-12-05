<template>
  <div>
    <h1>C++/Vue.js Cloud Storage</h1>
    <br>
    <input type="file" ref="fileUploading" v-on:change="selectedFileChange()">
    <b-button @click="uploadFile()" class="m-3 btn-info">
      <span v-if="uploadLoading" class="spinner-border spinner-border-sm"/>
      Upload File
    </b-button>
    <!-- Thanks to https://getbootstrap.com/docs/4.2/components/spinners/ !-->
    <div v-if="filesLoading">
      <br>
      <div class="spinner-border"/>
    </div>
    <span v-if="deleteLoading" class="spinner-border"/>
    <b-list-group v-if="files!=null">
      <b-list-group-item v-for="(file, index) in files" :key="index">
      <b-button class="btn-info" :href="'/file/'+file">{{file}}</b-button>
      <b-button @click="deleteFile(file)" class="m-3 btn-danger">Delete</b-button>
      </b-list-group-item>
    </b-list-group>
  </div>
</template>

<script>
import FileService from '../services/file-service'

export default {
  name: 'FileDisplay',
  data () {
    return {
      files: null,
      fileToUpload: null,
      filesLoading: true,
      uploadLoading: false,
      deleteLoading: false
    }
  },
  methods: {
    getFiles () {
      FileService.getFiles().then(response => {
        this.files = response.data
        this.filesLoading = false
      }).catch(e => {
        console.log(e)
      })
    },
    deleteFile (name) {
      this.deleteLoading = true
      FileService.deleteFile(name).then(response => {
        if (response.status === 200) {
          this.files = response.data
          this.deleteLoading = false
        }
      })
    },
    selectedFileChange (e) {
      this.fileToUpload = this.$refs.fileUploading.files[0]
    },
    uploadFile () {
      this.uploadLoading = true
      FileService.uploadFile(this.fileToUpload).then(response => {
        if (response.status === 200) {
          this.files = response.data
          this.uploadLoading = false
          this.$refs.fileUploading.value = null
        }
      })
    }
  },
  mounted () {
    this.getFiles()
  }
}
</script>

<style>
body{
  background-color:lightblue;
}
</style>
