<?php

use Illuminate\Support\Facades\Schema;
use Illuminate\Database\Schema\Blueprint;
use Illuminate\Database\Migrations\Migration;

class CreateKnowTable extends Migration
{
    /**
     * Run the migrations.
     *
     * @return void
     */
    public function up()
    {
        Schema::create('know', function (Blueprint $table) {
            $table->unsignedInteger('id_lang');
            $table->string('user');

            $table->primary(['id_lang', 'user']);
            $table->foreign('id_lang')->references('id')->on('languages');
            $table->foreign('user')->references('name')->on('users');
        });
    }

    /**
     * Reverse the migrations.
     *
     * @return void
     */
    public function down()
    {
        Schema::dropIfExists('know');
    }
}
