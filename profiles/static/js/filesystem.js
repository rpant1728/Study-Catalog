$(document).ready(function(){
    $(".course-id").val(course)
    $("#upload-files").on('click', function(){
        $(".course-id").val(course)
        $(".folder-id").val(folder)
    });
    $(document).on('click','#create-folder',function (e){
        title = $("#folder-title").val();
        course =  $("#course_id").val();
        folder = $("#folder_id").val();
        if(root){
            data = {
                'csrfmiddlewaretoken': token,
                'course_id': course,
                'title': title
            }
        }
        else{
            data = {
                'csrfmiddlewaretoken': token,
                'course_id': course,
                'title': title,
                'folder_id': folder
            }
        }
        $.ajax({
            type:"POST",
            url: folder_url,
            data: data,
            success: function(){
                $("#folders").load(location.href+" #folders");
            }
        });
    });

    $(document).on('click','.rename_resource',function (e){
        var $this = $(this);
        var id = $this.attr('id').split('-')[1];
        console.log("tp");
        var x = document.getElementById("title_resource").innerHTML;
        console.log(x);
        var filename = prompt("Please rename your File", x);
        filename = filename.trim();
        if (filename == null || filename == "")
            return; 
    
        $.ajax({
            type:"POST",
            url: rename_resource_url,
            data: {
                'csrfmiddlewaretoken': token,
                'resource_id' : id,
                'filename' : filename
            },
            success: function(result){
                document.getElementById("title_resource").innerHTML = filename;
            }
        });
        return false;
    });   

    $(document).on('click','.rename_folder',function (e){
        var $this = $(this);
        var id = $this.attr('id').split('-')[1];
        console.log("tp");
        var x = document.getElementById("title_folder").innerHTML;
        console.log(x);
        var foldername = prompt("Please rename your Folder", x);
        foldername = foldername.trim();
        if (foldername == null || foldername == "")
            return; 
    
        $.ajax({
            type:"POST",
            url: rename_folder_url,
            data: {
                'csrfmiddlewaretoken': token,
                'folder_id' : id,
                'foldername' : foldername
            },
            success: function(result){
                document.getElementById("title_folder").innerHTML = foldername;
            }
        });
        return false;
    }); 




});


