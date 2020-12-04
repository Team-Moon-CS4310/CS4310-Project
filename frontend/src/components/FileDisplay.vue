<template>
  <div>
    <h1>testtext</h1>
    <b-list-group>
    <b-list-group-item v-for="(file, index) in files"
    :key="index">
    <b-button :href="'/file/'+file">{{file}}</b-button>
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
      files: ['LOADING...']
    }
  },
  methods: {
    getFiles () {
      FileService.getFiles().then(response => {
        this.files = response.data
      }).catch(e => {
        console.log(e)
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
