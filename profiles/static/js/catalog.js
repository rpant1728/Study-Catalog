$(document).ready(function(){
    $(document).on('click','.approve-course',function (e){
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
                $('.course').load(location.href+" .course", "");
            }
        });
    });

    $(document).on('click','.reject-course',function (e){
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
                $('.course').load(location.href+" .course", "");
            }
        });
    });

    $(document).on('click','.approve-resource',function (e){
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
                $('.course').load(location.href+" .course", "");
            }
        });
    });

    $(document).on('click','.reject-resource',function (e){
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
                $('.course').load(location.href+" .course", "");
            }
        });
    });

    $(document).on('click','.approve-admin',function (e){
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
                $('.course').load(location.href+" .course", "");
            }
        });
    });

    $(document).on('click','.reject-admin',function (e){
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
                $('.course').load(location.href+" .course", "");
            }
        });
    });
});