$(document).ready(function(){
    $(".approve-course").on('click', function(){
        var $this = $(this);
        var id = $this.attr('id').split('-')[1];
        $.ajax({
            type:"POST",
            url: approve_course_url,
            data: {
                'csrfmiddlewaretoken': token,
                'course_id': id,
            },
            success: function(){
                $('.course-requests').load(location.href+" .course-requests", "");
            }
        });
    });

    $(".reject-course").on('click', function(){
        var $this = $(this);
        var id = $this.attr('id').split('-')[1];
        $.ajax({
            type:"POST",
            url: reject_course_url,
            data: {
                'csrfmiddlewaretoken': token,
                'course_id': id,
            },
            success: function(){
                $('.course-requests').load(location.href+" .course-requests", "");
            }
        });
    });

    $(".approve-resource").on('click', function(){
        var $this = $(this);
        var id = $this.attr('id').split('-')[1];
        $.ajax({
            type:"POST",
            url: approve_resource_url,
            data: {
                'csrfmiddlewaretoken': token,
                'resource_id': id,
            },
            success: function(){
                $('.resource-requests').load(location.href+" .resource-requests", "");
            }
        });
    });

    $(".reject-resource").on('click', function(){
        var $this = $(this);
        var id = $this.attr('id').split('-')[1];
        console.log(id);
        $.ajax({
            type:"POST",
            url: reject_resource_url,
            data: {
                'csrfmiddlewaretoken': token,
                'resource_id': id,
            },
            success: function(){
                $('.resource-requests').load(location.href+" .resource-requests", "");
            }
        });
    });

    $(".approve-admin").on('click', function(){
        var $this = $(this);
        var id = $this.attr('id').split('-')[1];
        $.ajax({
            type:"POST",
            url: approve_admin_url,
            data: {
                'csrfmiddlewaretoken': token,
                'user_id': id,
            },
            success: function(){
                $('.admin-requests').load(location.href+" .admin-requests", "");
            }
        });
    });

    $(".reject-admin").on('click', function(){
        var $this = $(this);
        var id = $this.attr('id').split('-')[1];
        console.log(id);
        $.ajax({
            type:"POST",
            url: reject_admin_url,
            data: {
                'csrfmiddlewaretoken': token,
                'user_id': id,
            },
            success: function(){
                $('.admin-requests').load(location.href+" .admin-requests", "");
            }
        });
    });
});