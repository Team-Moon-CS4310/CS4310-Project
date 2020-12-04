<template>
  <div>
    <b-list-group>
    <b-list-group-item v-for="(file, index) in files"
    :key="index">
    <b-button :href="'/files/'+file" class="float-right" style="background: #0d3103;">{{file}}</b-button>
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
      files: FileService.getFiles()
    }
  }
}
</script>

<style>
body{
  background-color:lightblue;
}
</style>
