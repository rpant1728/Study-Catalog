$(document).ready(function(){
    $(".course-id").val(course)
    $("#upload-files").on('click', function(){
        $(".course-id").val(course)
        $(".folder-id").val(folder)
    });
    $("#create-folder").on('click', function(){
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
});