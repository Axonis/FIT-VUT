@extends('layouts.app')

@section('content')
    <div class="container">
        <div class="row">
            <div class="col-md-8 col-md-offset-2">
                <div class="panel panel-default">
                    <div class="panel-heading">Module</div>

                    <div class="panel-body">
                        @if (!$module)
                            <h2>This bug does not exist!</h2>
                        @else

                            <form role="form" method="POST" action="{{ action('ModuleController@store') }}">
                                {{ csrf_field() }}

                                <div class="col-lg-4">
                                    <div class="form-group">
                                        <label>ID</label>
                                        <div class="radio">
                                            <input type="text" class="form-control" name="id" value="{{$module->id_modul}}" readonly>
                                        </div>
                                    </div>
                                </div>

                                <div class="col-lg-4">
                                    <div class="form-group">
                                            <label>Name</label>
                                            <div class="radio">
                                                <input type="text" class="form-control" name="name" value="{{$module->meno}}" disabled>
                                            </div>
                                    </div>
                                </div>

                                <div class="col-lg-4">
                                    <div class="form-group">
                                            <label>Programming language</label>
                                            <div class="radio">
                                                <input type="text" class="form-control" name="lan" value="{{$module->name}}" disabled>
                                            </div>
                                        </div>
                                </div>



                                <div class="col-md-12">
                                    <div class="form-group">
                                        <label>Error rate</label>
                                        <div class="radio">
                                            <input type="number" step="0.001" class="form-control" name="error" value="{{$module->chybovost}}"
                                                   @if(Auth::user()->group != 'Admin') disabled @endif
                                            >
                                        </div>
                                    </div>

                                    <div class="form-group">
                                        <label>Responsible person</label>
                                        <select class="form-control" name="responsible"
                                                @if(Auth::user()->group != 'Admin') disabled @endif
                                        >
                                            <option selected>{{$user->name}}</option>
                                            @foreach($alt_users as $us)
                                                @if($user->name != $us->name)
                                                    <option>{{$us->name}}</option>
                                                @endif
                                            @endforeach
                                        </select>
                                    </div>

                                    @if(Auth::user()->group == 'Admin')
                                        <div id="buttons">
                                            <button type="submit" class="btn btn-primary">Submit changes</button>
                                            <button type="reset" class="btn btn-default">Reset fields</button>
                                        </div>
                                    @endif
                                </div>




                            </form>
                        @endif

                    </div>
                </div>
            </div>
        </div>
    </div>
@endsection
