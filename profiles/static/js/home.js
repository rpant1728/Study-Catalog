$(document).ready(function(){
    $(".upvote").on('click', function(){
        var $this = $(this);
        var id = $this.attr('id').split('-')[1];
        function rgb2hex(rgb) {
            rgb = rgb.match(/^rgb\((\d+),\s*(\d+),\s*(\d+)\)$/);
            function hex(x) {
                return ("0" + parseInt(x).toString(16)).slice(-2);
            }
            return "#" + hex(rgb[1]) + hex(rgb[2]) + hex(rgb[3]);
        }
        if( rgb2hex($("#uparrow-"+id).css("color")) == "#ff0000" ){
            $.ajax({
                type:"POST",
                url: vote_url,
                data: {
                    'csrfmiddlewaretoken': token,
                    'post_id': id,
                    'upvote': "0",
                    'up_remove': "1",
                    'downvote' : "0",
                    'down_remove' : "0"
                },
                success: function(){
                    var votes = "" + (Number($("#votes-"+id).text())-1); 
                    $("#votes-"+id).text(votes);
                    $("#uparrow-"+id).css("color", "#666666");
                }
            });
        }else if( rgb2hex($("#downarrow-"+id).css("color")) == "#ff0000" ) {
            $.ajax({
                type:"POST",
                url: vote_url,
                data: {
                    'csrfmiddlewaretoken': token,
                    'post_id': id,
                    'upvote': "1",
                    'up_remove': "0",
                    'downvote' : "0",
                    'down_remove' : "1"
                },
                success: function(){
                    var votes = "" + (Number($("#votes-"+id).text())+2); 
                    $("#votes-"+id).text(votes);
                    $("#uparrow-"+id).css("color", "#ff0000");
                    $("#downarrow-"+id).css("color", "#666666");
                }
            });
        }else{
            $.ajax({
                type:"POST",
                url: vote_url,
                data: {
                    'csrfmiddlewaretoken': token,
                    'post_id': id,
                    'upvote': "1",
                    'up_remove': "0",
                    'downvote' : "0",
                    'down_remove' : "0"
                },
                success: function(){
                    var votes = "" + (Number($("#votes-"+id).text())+1); 
                    $("#votes-"+id).text(votes);
                    $("#uparrow-"+id).css("color", "#ff0000");
                }
            });
        }
        return false;
    });        

    $(".downvote").on('click', function(){
        var $this = $(this);
        var id = $this.attr('id').split('-')[1];
        function rgb2hex(rgb) {
            rgb = rgb.match(/^rgb\((\d+),\s*(\d+),\s*(\d+)\)$/);
            function hex(x) {
                return ("0" + parseInt(x).toString(16)).slice(-2);
            }
            return "#" + hex(rgb[1]) + hex(rgb[2]) + hex(rgb[3]);
        }
        if( rgb2hex($("#downarrow-"+id).css("color")) == "#ff0000" ){
            $.ajax({
                type:"POST",
                url: vote_url,
                data: {
                    'csrfmiddlewaretoken': token,
                    'post_id': id,
                    'upvote': "0",
                    'up_remove': "0",
                    'downvote' : "0",
                    'down_remove' : "1"
                },
                success: function(){
                    var votes = "" + (Number($("#votes-"+id).text())+1); 
                    $("#votes-"+id).text(votes);
                    $("#downarrow-"+id).css("color", "#666666");
                }
            });
        }else if( rgb2hex($("#uparrow-"+id).css("color")) == "#ff0000" ){
            $.ajax({
                type:"POST",
                url: vote_url,
                data: {
                    'csrfmiddlewaretoken': token,
                    'post_id': id,
                    'upvote': "0",
                    'up_remove': "1",
                    'downvote' : "1",
                    'down_remove' : "0"
                },
                success: function(){
                    var votes = "" + (Number($("#votes-"+id).text())-2); 
                    $("#votes-"+id).text(votes);
                    $("#downarrow-"+id).css("color", "#ff0000");
                    $("#uparrow-"+id).css("color", "#666666");
                }
            });
        }else{
            $.ajax({
                type:"POST",
                url: vote_url,
                data: {
                    'csrfmiddlewaretoken': token,
                    'post_id': id,
                    'upvote': "0",
                    'up_remove' : "0",
                    'downvote': "1",
                    'down_remove' : "0"
                },
                success: function(){
                    var votes = "" + (Number($("#votes-"+id).text())-1); 
                    $("#votes-"+id).text(votes);
                    $("#downarrow-"+id).css("color", "#ff0000");
                }
            });
        }
        return false;
    });     

    $(document).on('click','.delete_post',function (e){
        var $this = $(this);
        var post_id = $this.attr('id').split('-')[1];
        $.ajax({
            type:"POST",
            url: post_delete,
            data: {
                'csrfmiddlewaretoken': token,
                'post_id' : post_id
            },
            success: function(result){
                $("#post-" + post_id).hide();
            }
        });
        return false;
    });   

    $(document).on('click','.delete_comment',function (e){
        console.log("Test1");
        var $this = $(this);
        var comment_id = $this.attr('id').split('-')[1];
        $.ajax({
            type:"POST",
            url: comment_delete,
            data: {
                'csrfmiddlewaretoken': token,
                'comment_id' : comment_id
            },
            success: function(result){
                $("#comment-" + comment_id).hide();
            }
        });
        return false;
    });   

    $("#post").on('click', function(){
        var title = $('#title').val();
        var data = $('#data').val();
        $.ajax({
            type:"POST",
            url: post_url,
            data: {
                'csrfmiddlewaretoken': token,
                'title': title,
                'data': data
            },
            success: function(result){
                $('.posts').load(location.href+" .posts", "");
                document.getElementById("title").value = "";
                document.getElementById("data").value = "";
            }
        });
        return false;
    });

    $(document).on('click','.comment',function (e){
        var $this = $(this)
        var id = $this.attr('id').split('-')[1];
        var data = $('#data-'+id).val();
        var div=$('#comments-'+id).html()
        $.ajax({
            type:"POST",
            url: comment_url,
            data: {
                'csrfmiddlewaretoken': token,
                'post-id' : id,
                'data': data
            },
            success: function(result){
                $('#comments-'+id).load(location.href+" #comments-" +id);
            }
        });
        return false;
    });

    $('#create-course').on("click", function() {
        $('#course-form').show();
        $('#admin-request').hide();
    });

    $('#request-admin').on("click", function() {
        $('#course-form').hide();
        $('#admin-request').show();
    });

     $(document).on('click','.show_comments',function (e){
        var $this = $(this);
        var id = $this.attr('id').split('-')[1];
        if($this.val()=="0"){
            $("#com-"+id).attr('hidden', true);
            $this.val("1");
        }else{
            $("#com-"+id).attr('hidden', false);
            $this.val("0");
        }
    });

    $(".admin-req").on('click', function(){
        var $this = $(this)
        var id = $this.attr('id').split('-')[1];
        $.ajax({
            type:"POST",
            url: admin_request_url,
            data: {
                'csrfmiddlewaretoken': token,
                'user_id': id
            },
            success: function(result){
                $('#text').text("You have already requested to be an admin");
                $('#admin-'+id).hide();
            }
        });
        return false;
    });

    $("#select-course").change(function(){ 
        var course = $(this).val();  
        $.ajax({ 
          type: "POST", 
          url: "folders_url", 
          data: {
            'csrfmiddlewaretoken': token,
            'course': course,
          },
          success: function(result){ 
            $("#divid").load(" #divid");
          }
        });
  
      });
});
