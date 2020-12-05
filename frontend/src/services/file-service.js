import axios from 'axios'

class FileService {
  getFiles () {
    return axios.get('/getAllFiles')
  }

  deleteFile (name) {
    return axios.delete(name)
  }

  uploadFile (file) {
    var request = new FormData()
    request.append('file', file)
    return axios.post('/upload', request)
  }
}

export default new FileService()
