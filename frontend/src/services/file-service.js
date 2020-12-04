import axios from 'axios'

class FileService {
  getFiles () {
    axios.get('/getAllFiles').then(response => {
      console.log(response.data)
      return response.data
    })
      .catch(e => {
        console.log(e)
      })
  }
}

export default new FileService()
