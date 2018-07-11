@extends('layouts.app')
@section('content')
    <div class="container">
        <div class="row">
            <div class="col-md-8 col-md-offset-2">
                <div class="panel panel-default">
                    <div class="panel-heading">Show profile</div>

                    <div class="panel-body">

                        <form class="form-horizontal" role="form" method="POST" action="{{ action('ProfileController@store') }}">
                            {{ csrf_field() }}


                            <div class="form-group">
                                <label for="name" class="col-md-4 control-label">Username</label>
                                <div class="col-md-6">
                                    <input id="name" type="text" class="form-control" name="name" value="{{ $user->name }}" disabled>
                                </div>
                            </div>

                            <div class="form-group">
                                <label for="cele_meno" class="col-md-4 control-label">Full name</label>
                                <div class="col-md-6">
                                    <input id="cele_meno" type="text" class="form-control" name="cele_meno"
                                           disabled value=" {{ $user->cele_meno }}">
                                </div>
                            </div>

                            <div class="form-group">
                                <label for="vek" class="col-md-4 control-label">Age</label>

                                <div class="col-md-6">
                                    <input id="vek" type="text" class="form-control" name="vek"
                                           disabled value=" {{ $user->vek }}">
                                </div>
                            </div>

                            <div class="form-group">
                                <label for="languages" class="col-md-4 control-label">Programming Languages</label>

                                <div class="col-md-6">
                                    <input id="languages" type="text" class="form-control" name="languages"
                                           disabled value="{{$data}}">
                                </div>
                            </div>


                            <div class="form-group">
                                <label for="email" class="col-md-4 control-label">E-Mail Address</label>

                                <div class="col-md-6">
                                    <input id="email" type="email" class="form-control" name="email" value= {{ $user->email }} disabled>
                                </div>
                            </div>
                        </form>

                    </div>
                </div>
            </div>
        </div>
    </div>
@endsection
